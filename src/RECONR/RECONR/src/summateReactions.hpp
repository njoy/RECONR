// For incident neutrons
template< typename Range >
static
void summateReactions( const Logger& logger,
                       R2D2& r2d2,
                       const Range& energies ){

  auto AWR = r2d2.reactions().begin()->second.AWR();
  auto ZA = r2d2.reactions().begin()->second.ZA();

  const auto& proj = r2d2.projectile();
  const auto& target = r2d2.target();

  const elementary::ParticleID neutron{ "n" };

  auto& reactions = r2d2.reactions();
  auto& summations = r2d2.summations();

  auto fromENDF = [&]( auto&& mt ) -> elementary::ReactionID { 
    return fromEndfReactionNumber( proj, target, mt ); };

  auto transform = [&]( auto&& MTs ){
    std::vector< elementary::ReactionID > IDs;
    for( const auto& mt : MTs ){
      try {
        IDs.push_back( fromENDF( mt ) );
      } catch( std::exception& ){
        continue;
      }
    }
    return IDs;
  };

  auto sumRedundants = [&]( auto&& MT, auto&& redundantMTs ){
    logger.first << fmt::format( "MT: {}", MT ) << std::endl;

    std::vector< elementary::ReactionID > redundants;
    if( MT == 1 ){
      if( proj == neutron ){
        // Need to add in the MTs from MT=3 (not allowed for incident neutrons),
        // otherwise they won't be included in MT=1.
        auto mts = ranges::view::concat( 
          redundantMTs, ENDFtk::redundantReactions.at( 3 ) )
          | ranges::view::unique
          | ranges::to_vector;

        redundants = transform( mts );
      }
    } else if( MT == 3 ){
      // MT=3 not allowed for incident neutrons
      if( proj == neutron ){ return; }

    } else if( MT == 4 ){
      if( proj == neutron ){
        redundants = transform(
          redundantMTs
            // MT=50 is not allowed for incident neutrons
            | ranges::view::filter( []( auto&& mt ){ return mt != 50; } )
        );
      }
    }

    if( ranges::distance( redundants ) == 0 ){
      redundants = transform( redundantMTs );
    }

    std::vector< std::vector< double > > partials;
    for( const auto& id : redundants ){
      auto mt = elementary::toEndfReactionNumber( id );
    
      std::vector< double > partial;
      if ( summations.count( id ) > 0 ) {
        logger.first << fmt::format( "\t{:3}, {}", mt, id.symbol()  ) 
                     << std::endl;
        partial = summations.at( id ).template crossSections< XSPair >().second;
      } else if( reactions.count( id ) > 0 ){
        logger.first << fmt::format( "\t{:3}, {}", mt, id.symbol()  ) 
                     << std::endl;
        partial = reactions.at( id ).template crossSections< XSPair >().second;
      } // else { return; } // no existing partial

      if( not partial.empty() ){ partials.push_back( partial ); }
      
    } // for id

    if( partials.size() > 0 ){
      auto sumID = elementary::fromEndfReactionNumber( proj, target, MT );

      // Simply move to summations
      if( ( partials.size() == 1 ) and ( reactions.count( sumID ) > 0 ) ){
        for( const auto& ID : ranges::view::keys( reactions ) ){
          auto mt = elementary::toEndfReactionNumber( ID );
        }
        auto reaction = reactions.at( sumID );
        summations.emplace( sumID, std::move( reaction ) );
        reactions.erase( sumID );
      } else{
        auto rPair = std::make_pair( utility::copy( energies ), 
                                    sumPartials( partials ) );
        summations.emplace( sumID,
          Reaction{ ZA, AWR, 0.0, 0.0, 0, std::move( rPair ) } );
      }
    } // partials size
  }; // sumRedundants

  // Sum redundant cross sections
  logger.first << "Summing redundant cross sections\n";
  for( auto& [ MT, redundantMTs ] : 
       ranges::view::reverse( ENDFtk::redundantReactions ) ){

    // We have to do these at the end
    if( ( MT == 1 ) or ( MT == 3 ) or ( MT == 27 ) ){ continue; }
    sumRedundants( MT, redundantMTs );
  } // for MT

  sumRedundants( 27, ENDFtk::redundantReactions.at( 27 ) );
  sumRedundants( 3, ENDFtk::redundantReactions.at( 3 ) );
  sumRedundants( 1, ENDFtk::redundantReactions.at( 1 ) );
}
