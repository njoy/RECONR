// For incident neutrons
template< typename Range >
static
void summateReactions( std::ostream& output,
                       std::ostream& error,
                       R2D2::XSMap_t& reactions,
                       const R2D2::ReconMap_t& reconstructed,
                       const Range& energies ){


  output << "\nCalculating cross sections on unionized energy grid for IDs:\n";
  // Reconstruct everything
  // Perhaps this could be more efficient by only reconstructing those things
  // that are used, but I doubt that is the limiting factor here
  for( auto& [ ID, reaction ] : reactions ){
    output << fmt::format( "{:3s} ", ID );
    
    auto barns =  energies 
      | ranges::view::transform( 
          reaction.crossSections< interp::LinearTable >() )
      | ranges::to_vector;

    reaction.crossSections( 
      std::make_pair( utility::copy( energies ), barns ) );
  }

  // Add reconstructed resonances
  output << "\nAdding reconstructed cross sections to background for IDs:\n";
  for( const auto& ID : ranges::view::keys( reconstructed ) ){
    std::vector< std::vector< double >  > partials;

    // std::unique_ptr< Reaction > reaction = nullptr;
    // using PType = R2D2::ReconMap_t::mapped_type;
    // std::unique_ptr< PType > recon = nullptr;

    auto addReconstructed = [&]( const ReactionID& rxnID, 
                                 const ReactionID& reconID ){
      output << fmt::format( "{:3s} ", rxnID );
      auto& recon = reconstructed.at( reconID );
      auto& reaction = reactions.at( rxnID );
      auto& part = reaction.template crossSections< Rxn::Pair >().second;
      partials |= ranges::action::push_back( part );

      for( const auto& XS : recon ){
        auto partial = energies | ranges::view::transform( XS );
        partials |= ranges::action::push_back( partial );
      }
      auto sum = sumPartials( partials );
      reaction.crossSections( std::make_pair( utility::copy( energies ), 
                                              sum ) );
      auto xs = reaction.template crossSections< Rxn::Pair >().second;
    };

    // Add resonance data to ID=19 if there are partials
    if( ID == "18" ){
      if( reactions.find( "19" ) != reactions.end() ){
        addReconstructed( "19", "18" );
      } else if (reactions.find( "18" ) != reactions.end() ) {
        addReconstructed( "18", "18" );
      }
    } else{
      if( reactions.find( ID ) != reactions.end() ){
        addReconstructed( ID, ID );
      }
    }
    output << std::endl;
  } // Reconstructed resonances

  // Sum redundant cross sections
  output << "Summing redundant cross sections for IDs:\n";
  for( const auto& [ MT, redundantMTs ] : 
       ranges::view::reverse( ENDFtk::redundantReactions ) ){

    auto ID = MT2ReactionID( MT );
    auto redundants = redundantMTs
      | ranges::view::filter( 
        [&]( auto&& mt ){ return reactions.count( MT2ReactionID( mt ) ) > 0; } )
      | ranges::view::transform( 
        []( auto&& mt ){ return MT2ReactionID( mt ); } );

    if( ranges::distance( redundants ) != 0 ){
      output << fmt::format( "{:3d}, redundant IDs: ", MT );
      
      std::vector< std::vector< double > > partials;
      for( const auto& p : redundants ){
        output << fmt::format( "{:3s} ", p );

        partials 
          |= ranges::action::push_back( 
              reactions.at( p ).template crossSections< Rxn::Pair >().second );

      } // redundants

      auto rPair = std::make_pair( utility::copy( energies ), 
                                   sumPartials( partials ) );
      try {
        auto found = reactions.find( ID );
        if( found != reactions.end() ){
          reactions.at( ID ).crossSections( std::move( rPair ) );
        } else {
          // New reaction made entirely of summations
          auto id = redundants.front();
          auto rxn = reactions.at( id );
          reactions.emplace( ID, 
            Reaction{ rxn.ZA(), rxn.AWR(), 0.0, 0.0, 0, std::move( rPair ) }
          );
        }
      } catch( std::exception& ){
        Log::error( "'total' cross section not orginally given for ID: {}",
                   ID );
        throw;
      }
      output << std::endl;
    }
  } // Redundant cross sections

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
  output << std::endl;

  return productions;
}
