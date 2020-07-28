// For incident neutrons
template< typename Range >
static
void summateReactions( std::ostream& output,
                       std::ostream& error,
                       R2D2& r2d2,
                       const Range& energies ){

  auto AWR = r2d2.reactions().begin()->second.AWR();
  auto ZA = r2d2.reactions().begin()->second.ZA();

  const auto& proj = r2d2.projectile();
  const auto& target = r2d2.target();

  const elementary::ParticleID neutron{ "n" };

  const auto& reactions = r2d2.reactions();
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
  // Sum redundant cross sections
  output << "Summing redundant cross sections\n";
  for( auto& [ MT, redundantMTs ] : 
       ranges::view::reverse( ENDFtk::redundantReactions ) ){

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
      if( proj == neutron ){
        continue;
      }
    } else if( MT == 4 ){
      if( proj == neutron ){
        redundants = transform(
          redundantMTs
            // MT=50 is not allowed for incident neutrons
            | ranges::view::filter( []( auto&& mt ){ return mt != 50; } )
        );
      }
    } else { redundants = transform( redundantMTs ); }

    std::vector< std::vector< double > > partials;
    for( const auto& id : redundants ){
    
      std::vector< double > partial;
      if ( summations.count( id ) > 0 ) {
        partial = summations.at( id ).template crossSections< XSPair >().second;
      } else if( reactions.count( id ) > 0 ){
        partial = reactions.at( id ).template crossSections< XSPair >().second;
      } else { continue; } // no existing partial

      partials.push_back( partial );
      
    } // for id

    if( partials.size() > 0 ){
      auto rPair = std::make_pair( utility::copy( energies ), 
                                  sumPartials( partials ) );
      summations.emplace(
        elementary::fromEndfReactionNumber( proj, target, MT ),
        Reaction{ ZA, AWR, 0.0, 0.0, 0, std::move( rPair ) } );
    }
  } // for MT
}
