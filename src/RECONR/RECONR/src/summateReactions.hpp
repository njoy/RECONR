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

    auto addReconstructed = [&]( const ReactionID& rxnID, 
                                 const ReactionID& reconID ){
      output << fmt::format( "{:3s} ", rxnID );
      auto& recon = reconstructed.at( reconID );
      auto& reaction = reactions.at( rxnID );
      auto& part = reaction.template crossSections< XSPair >().second;
      partials.push_back( part );

      for( const auto& XS : recon ){
        auto partial = energies | ranges::view::transform( XS );
        partials.push_back( partial );
      }
      auto sum = sumPartials( partials );
      reaction.crossSections( std::make_pair( utility::copy( energies ), 
                                              sum ) );
      auto xs = reaction.template crossSections< XSPair >().second;
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

        partials.push_back(
              reactions.at( p ).template crossSections< XSPair >().second );

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
void summateReactions( std::ostream& output,
                       std::ostream& error,
                       R2D2::PPMap_t& productions,
                       const Range& energies ){

  if( productions.size() <= 0 ){
    return;
  }

  output << 
    "\nSumming photon production cross sections on unionized energy grid"
    " for IDs: \n";
  for( auto& [ ID, production ] : productions ){
    output << fmt::format( "{:3s} ", ID );

    std::vector< std::vector< double > > partials;
    for( const auto& partial : 
         production.template productions< interp::LinearTable >() ){
      auto barns =  energies 
        | ranges::view::transform( partial )
        | ranges::to_vector;

      partials.emplace_back( barns );
    }
    auto sum = sumPartials( partials );

    auto pairs = ranges::view::zip_with( 
      []( auto&& e, auto&& p ){ return PPForms{ std::make_pair( e, p ) }; },
      energies, sum )
      | ranges::to_vector;
    production.productions( std::move( pairs ) );
  }
  output << std::endl;
}


template< typename Range >
static
void summateReactions( std::ostream& output,
                       std::ostream& error,
                       R2D2& data,
                       const Range& energies ){
  summateReactions( 
    output, error, data.reactions(), data.reconstructedResonances(), energies );
  summateReactions( output, error, data.photonProductions(), energies );
}
