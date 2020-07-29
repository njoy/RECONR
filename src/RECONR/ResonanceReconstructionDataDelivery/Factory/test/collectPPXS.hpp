SCENARIO( "Testing the collection of photon production cross sections" ){
  GIVEN( "an ENDF Material" ){
    auto material = details::ENDFMaterial( "SLBW", false );

    WHEN( "the photon production cross sections have been extracted" ){
      njoy::elementary::ParticleID proj{ "n" };
      njoy::elementary::ParticleID target{ "fe56" };
      auto reactions = TFactory::collectPPXS( material, proj, target );
      
      using PID = ParticleID;
      using NID = NucleusID;

      std::vector< njoy::RECONR::ReactionID > IDs{ 
        ReactionID{ proj, target, ReactionType{ 3 } },
        ReactionID{ proj, target, ReactionType{ 18 } },
      };
      auto keys = ranges::view::keys( reactions ) | ranges::to_vector;
      std::sort( IDs.begin(), IDs.end() );
      std::sort( keys.begin(), keys.end() );

      CHECK( ranges::equal( IDs, keys ) );

      THEN( "MT=3 can be checked" ){

        ReactionID reactionID{ PID{ "n" }, PID{ "fe56" }, ReactionType{ 3 } };
        auto reaction = reactions.at( reactionID );

        CHECK( 92235 == reaction.ZA() );
        CHECK( 233.0248 == reaction.atomicWeightRatio() );

        std::vector< double > refE{
          1.00E-5, 1.00E-1, 1.09E+6, 1.09E+6, 2.00E+6, 5.00E+6, 5.50E+6,
          6.00E+6, 6.50E+6, 7.00E+6, 7.50E+6, 8.00E+6, 1.42E+7, 2.00E+7
        };
        std::vector< double > refB{ 0.0, 0.0, 0.0, 1.3, 2.3, 3.3, 4.3, 5.3,
                                    6.3, 7.3, 8.3, 9.3, 10.3, 11.3 };

        auto production = 
          reaction.productions< std::vector< interp::Variant > >()[ 0 ];

        auto xs = std::get< njoy::RECONR::interp::LinearLinear >( 
          production[ 0 ] );
        auto energies = xs.x() | ranges::to_vector;
        auto barns = xs.y() | ranges::to_vector;

        details::checkRanges( refE, energies );
        details::checkRanges( refB, barns );
      } // THEN
      THEN( "MT=18 can be checked" ){

        ReactionID reactionID{ PID{ "n" }, PID{ "fe56" }, ReactionType{ 18 } };
        auto reaction = reactions.at( reactionID );

        CHECK( 92238 == reaction.ZA() );
        CHECK( 245.0248 == Approx( reaction.atomicWeightRatio() ) );

        std::vector< double > refE{ 1.0E-5, 3.0E7 };
        std::vector< double > refB{ 10.18, 11.18 };

        auto production = 
          reaction.productions< std::vector< interp::Variant > >()[ 0 ];

        auto xs = std::get< njoy::RECONR::interp::LinearLinear >( 
          production[ 0 ] );
        auto energies = xs.x() | ranges::to_vector;
        auto barns = xs.y() | ranges::to_vector;

        details::checkRanges( refE, energies );
        details::checkRanges( refB, barns );
      } // THEN
    } // WHEN
    
  } // GIVEN
  
} // SCENARIO
