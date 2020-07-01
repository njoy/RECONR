// For incident neutrons
template< typename Range >
static
auto summateReactions( std::ostream& output,
                       const R2D2::LinMap_t& linear,
                       const R2D2::ReconMap_t& reconstructed,
                       const Range& energies ){
  using Reaction_t = Reaction< std::vector< double > >;
  using RMap_t = std::map< ReactionID, Reaction_t >;
  RMap_t reactions;

  auto keys = ranges::view::keys( linear );
  auto recKeys = ranges::view::keys( reconstructed );

  output << "\nCalculating cross sections on unionized energy grid for IDs:\n";
  // Reconstruct everything
  // Perhaps this could be more efficient by only reconstructing those things
  // that are used, but I doubt that is the limiting factor here
  for( const auto& [ ID, reaction ] : linear ){
    output << fmt::format( "{:3s} ", ID );
    
    auto barns =  energies 
      | ranges::view::transform( reaction.crossSections() )
      | ranges::to_vector;
    reactions.emplace( ID, Reaction_t{ reaction, std::move( barns ) } );
  }

  // Add reconstructed resonances
  output << "\nAdding reconstructed cross sections to background for IDs:\n";
  for( const auto& ID : ranges::view::keys( reconstructed ) ){
    std::vector< std::vector< double >  > partials;

    std::unique_ptr< Reaction_t > reaction = nullptr;
    using PType = R2D2::ReconMap_t::mapped_type;
    std::unique_ptr< PType > recon = nullptr;

    // Add resonance data to ID=19 if there are partials
    if( ID == "18" ){
      if( reactions.find( "19" ) != reactions.end() ){
        output << fmt::format( "{:3s} ", "19" );
        recon = std::make_unique< PType >( reconstructed.at( "18" ) );
        reaction = std::make_unique< Reaction_t >( reactions.at( "19" ) );
        partials |= ranges::action::push_back( reaction->crossSections() );
      }
    }
    if( recon == nullptr ){
      if( reactions.find( ID ) != reactions.end() ){
        output << fmt::format( "{:3s} ", ID );
        recon = std::make_unique< PType >( reconstructed.at( ID ) );
        reaction = std::make_unique< Reaction_t >( reactions.at( ID ) );
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
    reactions.insert_or_assign( ID, std::move( rReaction ) );
  } // Reconstructed resonances

  // Sum redundant cross sections
  output << "\nSumming redundant cross sections for IDs:\n";
  for( const auto& [ ID, redundantIDs ] : 
       ranges::view::reverse( ENDFtk::redundantReactions ) ){

    auto redundants = redundantIDs
      | ranges::view::filter( 
        [&]( auto&& mt ){ return reactions.count( MT2ReactionID( mt ) ) > 0; } )
      | ranges::view::transform( 
        []( auto&& mt ){ return MT2ReactionID( mt ); } );

    if( ranges::distance( redundants ) != 0 ){
      output << fmt::format( "{:3d}, redundant IDs: ", ID );
      
      std::vector< std::vector< double > > partials;
      for( const auto& p : redundants ){
        output << fmt::format( "{:3s} ", p );

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
        reactions.insert_or_assign( MT2ReactionID( ID ), 
                                    std::move( sReaction ) );
      } // redundants
      output << std::endl;
    }

  } // Redundant cross sections
  return reactions;
}

// For Photon production reactions
template< typename Range >
static
auto summateReactions( std::ostream& output,
                       const R2D2::PPLinMap_t& linear,
                       const Range& energies ){

  using Production_t = PPReaction< double >;
  using PMap_t = std::map< ReactionID, Production_t >;
  PMap_t productions;

  output << 
    "\nCalculating photon production cross sections on unionized energy grid"
    " for IDs: \n";
  for( const auto& [ ID, reaction ] : linear ){
    output << fmt::format( "{:3s} ", ID );

    std::vector< std::vector< double > > partials;
    for( const auto& partial : reaction.productions() ){
      auto barns =  energies 
        | ranges::view::transform( partial )
        | ranges::to_vector;

      partials.emplace_back( barns );
    }
    Production_t prod{ reaction, sumPartials( partials ) };
    productions.emplace( ID,
                         Production_t{ reaction, sumPartials( partials ) } );
  }

  return productions;
}
