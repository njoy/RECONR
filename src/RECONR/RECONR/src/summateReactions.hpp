template< typename Range >
static
auto summateReactions( ResonanceReconstructionDataDelivery& r2d2,
                       Range& energies ){
  using Reaction_t = Reaction< std::vector< double > >;
  std::map< int, Reaction_t > reactions;
  
  const auto linear = r2d2.linearReactions();
  const auto reconstructed = r2d2.reconstructedResonances();

  auto keys = ranges::view::keys( linear );
  auto recKeys = ranges::view::keys( reconstructed );

  Log::info( "Calculating cross sections on unionized energy grid for MTs: " );
  // Reconstruct everything
  // Perhaps this could be more efficient by only reconstructing those things
  // that are used, but I doubt that is the limiting factor here
  for( const auto& [ MT, reaction ] : linear ){
    Log::info( "\t{}", MT );
    
    auto barns =  energies 
      | ranges::view::transform( reaction.crossSections() )
      | ranges::to_vector;
    reactions.emplace( MT, Reaction_t{ reaction, std::move( barns ) } );
  }

  // Add reconstructed resonances
  Log::info( "Adding reconstructed cross sections to background for MTs: " );
  for( const auto& MT : ranges::view::keys( reconstructed ) ){
    std::vector< std::vector< double >  > partials;

    using RType = decltype( reactions )::mapped_type;
    std::unique_ptr< RType > reaction = nullptr;
    using PType = decltype( reconstructed )::mapped_type;
    std::unique_ptr< PType > recon = nullptr;

    // Add resonance data to MT=19 if there are partials
    if( MT == 18 ){
      if( reactions.find( 19 ) != reactions.end() ){
        Log::info( "\t{}", 19 );
        recon = std::make_unique< PType >( reconstructed.at( 19 ) );
        reaction = std::make_unique< RType >( reactions.at( 19 ) );
        partials |= ranges::action::push_back( reaction->crossSections() );
      }
    }
    if( recon == nullptr ){
      if( reactions.find( MT ) != reactions.end() ){
        Log::info( "\t{}", MT );
        recon = std::make_unique< PType >( reconstructed.at( MT ) );
        reaction = std::make_unique< RType >( reactions.at( MT ) );
        partials |= ranges::action::push_back( reaction->crossSections() );
      }
    }

    // There isn't a background cross section to which we can add the 
    // reconstructed cross sections
    if( recon == nullptr ) continue;

    // Get cross sections from reconstructed reactions
    for( const auto& XS : *recon ){
      auto partial = energies | ranges::view::transform( XS );
      partials |= ranges::action::push_back( partial );
    }
    Reaction_t rReaction{ *reaction, sumPartials( partials ) };
    reactions.insert_or_assign( MT, std::move( rReaction ) );
  } // Reconstructed resonances

  // Sum redundant cross sections
  Log::info( "Summing redundant cross sections for MTs: " );
  for( const auto& [ MT, redundantMTs ] : 
       ranges::view::reverse( ENDFtk::redundantReactions ) ){

    if( MT == 27 ) continue;

    auto redundants = redundantMTs
      | ranges::view::filter( 
        [&]( auto&& mt ){ return reactions.count( mt ) > 0; } );

    if( ranges::distance( redundants ) != 0 ){
      Log::info( "\t{}, redundant MTs:", MT );
      
      std::vector< std::vector< double > > partials;
      for( const auto& p : redundants ){
        Log::info( "\t\t{}", p );

        // Look for already reconstructed reactions
        auto found = reactions.find( p );
        if( found != reactions.end() ){
          partials 
            |= ranges::action::push_back( found->second.crossSections() );
        } else{
          // Calculate the summed reaction since it doesn't already exist
          auto foundReaction = linear.find( p );
          if( foundReaction != linear.end() ){
            auto part = foundReaction->second.crossSections();
            auto party = part.x() 
              | ranges::view::transform( part ) 
              | ranges::to_vector;
            partials |= ranges::action::push_back( party );
          }
        }

        Reaction_t sReaction{ reactions.at( p ), sumPartials( partials ) };
        reactions.insert_or_assign( MT, std::move( sReaction ) );
      } // redundants
    }
  } // Redundant cross sections
  return reactions;
}
