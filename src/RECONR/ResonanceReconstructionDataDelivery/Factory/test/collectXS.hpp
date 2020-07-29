SCENARIO( "Testing the the collection of cross sections" ){
  GIVEN( "an ENDF Material" ){
    auto material = details::ENDFMaterial( "SLBW", false );

    WHEN( "the resonance reconstruction data is extracted" ){
      
      ParticleID proj{ "n" };
      ParticleID target{ "fe56" };
      auto reactions = TFactory::collectXS( material, proj, target );

      using PID = ParticleID;
      using NID = NucleusID;

      std::vector< njoy::RECONR::ReactionID > IDs{ 
        // ReactionID{ proj, target, ReactionType{ 1 } },
        ReactionID{ proj, target, ReactionType{ 2 } },
        // ReactionID{ proj, target, ReactionType{ 16 } },
        ReactionID{ proj, target, ReactionType{ 18 } },
        ReactionID{ proj, target, ReactionType{ 51 } },
        ReactionID{ proj, target, ReactionType{ 52 } },
        ReactionID{ proj, target, ReactionType{ 102 } },
        ReactionID{ proj, target, ReactionType{ 875 } },
        ReactionID{ proj, target, ReactionType{ 876 } },
        ReactionID{ proj, target, ReactionType{ 877 } }
      };
      auto keys = ranges::view::keys( reactions ) | ranges::to_vector;
      std::sort( IDs.begin(), IDs.end() );
      std::sort( keys.begin(), keys.end() );

      CHECK( ranges::equal( IDs, keys ) );

      THEN( "MT=2 can be checked" ){ 
        ReactionID reactionID{ PID{ "n" }, PID{ "fe56" }, ReactionType{ 2 } };
        auto reaction = reactions.at( reactionID );

        CHECK( 1002 == reaction.ZA() );
        CHECK( 0.9991673 == reaction.AWR() );
        CHECK( 2.224648E2 == reaction.QM() );
        CHECK( 3.224648E2 == reaction.QI() );
        CHECK( 0 == reaction.LR() );

        auto xs = std::get< njoy::RECONR::interp::Histogram >( 
            reaction.crossSections< std::vector< interp::Variant > >()[ 0 ] );
        std::vector< double > refE{
          1.0E-5, 2.0E-5, 7.5E+5, 1.9E+7, 1.95E+7, 2.0E+7 };
        std::vector< double > refB( refE.size(), 2.0 );

        auto energies = xs.x() | ranges::to_vector;
        auto barns = xs.y() | ranges::to_vector;

        details::checkRanges( refE, energies );
        details::checkRanges( refB, barns );

        CHECK( 2.0 == Approx( xs( 1.5E-5 ) ) );
        CHECK( 2.0 == Approx( xs( 1.999999999E-5 ) ) );
        CHECK( 2.0 == Approx( xs( 2.0E-5 ) ) );
        CHECK( 2.0 == Approx( xs( 1 ) ) );
        CHECK( 2.0 == Approx( xs( 8E5 ) ) );
        CHECK( 2.0 == Approx( xs( 1.925E7 ) ) );
        CHECK( 2.0 == Approx( xs( 1.975E7 ) ) );
      }
      THEN( "MT=18 can be checked" ){ 
        ReactionID reactionID{ PID{ "n" }, PID{ "fe56" }, ReactionType{ 18 } };
        auto reaction = reactions.at( reactionID );

        CHECK( 1018 == reaction.ZA() );
        CHECK( 0.9991673 == reaction.AWR() );
        CHECK( 2.224648E2 == reaction.QM() );
        CHECK( 3.224648E2 == reaction.QI() );
        CHECK( 0 == reaction.LR() );

        {
          auto xs1 = std::get< njoy::RECONR::interp::Histogram >(
              reaction.crossSections< std::vector< interp::Variant > >()[ 0 ] );
          std::vector< double > refE{ 1.0E+5, 1.5E+5, 7.5E+5 };
          std::vector< double > refB{ 1.8E+1, 1.182897E+1, 3.347392E-5 };

          auto energies = xs1.x() | ranges::to_vector;
          auto barns = xs1.y() | ranges::to_vector;

          details::checkRanges( refE, energies );
          details::checkRanges( refB, barns );

          CHECK( 18.0 == Approx( xs1( 1.25E+5 ) ) );
          CHECK( 1.182897E1 == Approx( xs1( 2E+5 ) ) );
          CHECK( 3.347392E-5 == Approx( xs1( 7.5E5 ) ).margin( 1E-7 ) );
        }
        {
          auto xs2 = std::get< njoy::RECONR::interp::LinearLinear >(
              reaction.crossSections< std::vector< interp::Variant > >()[ 1 ] );
          std::vector< double > refE{ 7.5E+5, 1.9E+7, 1.95E+7, 2.0E+7 };
          std::vector< double > refB{ 
            3.347392E-5, 2.751761E-5, 2.731301E-5, 2.710792E-5 };

          auto energies = xs2.x() | ranges::to_vector;
          auto barns = xs2.y() | ranges::to_vector;

          details::checkRanges( refE, energies );
          details::checkRanges( refB, barns );

          auto refY = linlinInterpolation( 
            1.925E7, refE[ 0 ], refE[ 1 ], refB[ 0 ], refB[ 1 ] );
          CHECK( refY == Approx( xs2( 1.925E7 ) ) );

          refY = linlinInterpolation( 
            1.975E7, refE[ 1 ], refE[ 2 ], refB[ 1 ], refB[ 2 ] );
          CHECK( refY == Approx( xs2( 1.975E7 ) ) );
        }
      }
      THEN( "MT=102 can be checked" ){ 
        ReactionID reactionID{ PID{ "n" }, PID{ "fe56" }, ReactionType{ 102 } };
        auto reaction = reactions.at( reactionID );

        CHECK( 1102 == reaction.ZA() );
        CHECK( 0.9991673 == reaction.AWR() );
        CHECK( 2.224648E6 == reaction.QM() );
        CHECK( 3.224648E6 == reaction.QI() );
        CHECK( 0 == reaction.LR() );

        {
          auto xs102 = std::get< njoy::RECONR::interp::LinearLogarithmic >( 
              reaction.crossSections< std::vector< interp::Variant > >()[ 0 ] );
          std::vector< double > refE{ 1.0E-5, 2.0E-5, 7.5E+5 };
          std::vector< double > refB{ 1.02E+2, 1.182897E+1, 3.347392E-5 };

          auto energies = xs102.x() | ranges::to_vector;
          auto barns = xs102.y() | ranges::to_vector;

          details::checkRanges( refE, energies );
          details::checkRanges( refB, barns );

          double refY = linlogInterpolation( 
            1.5E-5, refE[ 0 ], refE[ 1 ], refB[ 0 ], refB[ 1 ] );
          CHECK( refY == Approx( xs102( 1.5E-5 ) ).margin( 1E-7 ) );

          refY = linlogInterpolation( 
            1, refE[ 1 ], refE[ 2 ], refB[ 1 ], refB[ 2 ] );
          CHECK( refY == Approx( xs102( 1 ) ).margin( 1E-7 ) );
        }
        {
          auto xs4 = std::get< njoy::RECONR::interp::LogarithmicLinear >( 
              reaction.crossSections< std::vector< interp::Variant > >()[ 1 ] );
          std::vector< double > refE{ 7.5E+5, 1.9E+7, 1.95E+7, 2.0E+7 };
          std::vector< double > refB{
             3.347392E-5, 2.751761E-5, 2.731301E-5, 2.710792E-5 };

          auto energies = xs4.x() | ranges::to_vector;
          auto barns = xs4.y() | ranges::to_vector;

          details::checkRanges( refE, energies );
          details::checkRanges( refB, barns );

          auto refY = loglinInterpolation( 
            8E5, refE[ 0 ], refE[ 1 ], refB[ 0 ], refB[ 1 ] );
          CHECK( refY == Approx( xs4( 8E5 ) ).margin( 1E-7 ) );

          refY = loglinInterpolation( 
            1.925E7, refE[ 1 ], refE[ 2 ], refB[ 1 ], refB[ 2 ] );
          CHECK( refY == Approx( xs4( 1.925E7 ) ).margin( 1E-7 ) );

          refY = loglinInterpolation( 
            1.975E7, refE[ 2 ], refE[ 3 ], refB[ 2 ], refB[ 3 ] );
          CHECK( refY == Approx( xs4( 1.975E7 ) ).margin( 1E-7 ) );
        }
      }
    } // WHEN
  } // GIVEN
} // SCENARIO
