SCENARIO( "Testing the summation of cross sections" ){
  double absTol{ 1E-6 };
  double relTol{ 1E-1 }; // This tolerance is large by design

  using RPair = njoy::RECONR::XSPair;
  using PPair = njoy::RECONR::PPair;

  GIVEN( "a SpecialCase object" ){
    auto [energies, r2d2] = lin_recon( "SpecialCase", absTol, relTol );
    auto sizeEnergies = ranges::distance( energies );

    WHEN( "cross sections have been summated" ){
      tRECONR::summateReactions( logger, r2d2, energies );

      auto projectile = r2d2.projectile();
      auto target = r2d2.target();

      auto reactions = r2d2.reactions();
      auto summations = r2d2.summations();

      THEN( "the reactions can be summed up and checked" ){
        std::vector< njoy::RECONR::ReactionID > refReactions{ 
          ReactionID{ projectile, target, ReactionType{ 1 } },
          ReactionID{ projectile, target, ReactionType{ 2 } },
          ReactionID{ projectile, target, ReactionType{ 18 } },
          ReactionID{ projectile, target, ReactionType{ 51 } },
          ReactionID{ projectile, target, ReactionType{ 52 } },
          ReactionID{ projectile, target, ReactionType{ 102 } },
          ReactionID{ projectile, target, ReactionType{ 875 } },
          ReactionID{ projectile, target, ReactionType{ 876 } },
          ReactionID{ projectile, target, ReactionType{ 877 } }
        };
        auto rxKeys = ranges::view::keys( reactions ) | ranges::to_vector;
        std::sort( refReactions.begin(), refReactions.end() );
        std::sort( rxKeys.begin(), rxKeys.end() );

        CHECK( ranges::equal( refReactions, rxKeys ) );

        std::vector< njoy::RECONR::ReactionID > refSummations{ 
          ReactionID{ projectile, target, ReactionType{ 1 } },
          ReactionID{ projectile, target, ReactionType{ 4 } },
          ReactionID{ projectile, target, ReactionType{ 16 } },
          ReactionID{ projectile, target, ReactionType{ 27 } },
          ReactionID{ projectile, target, ReactionType{ 101 } }
        };
        auto smKeys = ranges::view::keys( summations ) | ranges::to_vector;
        std::sort( refSummations.begin(), refSummations.end() );
        std::sort( smKeys.begin(), smKeys.end() );

        CHECK( ranges::equal( refSummations, smKeys ) );
      } // THEN
      THEN( "MT = 1 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 1 } };

        auto mt2 = ReactionID{ projectile, target, ReactionType{ 2 } };
        auto mt4 = ReactionID{ projectile, target, ReactionType{ 4 } };
        auto mt16 = ReactionID{ projectile, target, ReactionType{ 16 } };
        auto mt27 = ReactionID{ projectile, target, ReactionType{ 27 } };
        // Don't need 101 because it is included in 27

        std::vector< double  > refXS = sumRanges(
          reactions.at(  mt2   ).template crossSections< RPair >().second,
          summations.at( mt4   ).template crossSections< RPair >().second,
          summations.at( mt16  ).template crossSections< RPair >().second,
          summations.at( mt27  ).template crossSections< RPair >().second
        );

        auto reaction = summations.at( ID );
        details::checkRanges( refXS, reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 4 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 4 } };
        auto mt51 = ReactionID{ projectile, target, ReactionType{ 51 } };
        auto mt52 = ReactionID{ projectile, target, ReactionType{ 52 } };
        std::vector< double > refXS = sumRanges( 
            reactions.at( mt51 ).template crossSections< RPair >().second, 
            reactions.at( mt52 ).template crossSections< RPair >().second );

        auto reaction = summations.at( ID );
        CHECK( refXS == reaction.crossSections< RPair >().second );

      } // THEN
      THEN( "MT = 16 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 16 } };
        auto mt875 = ReactionID{ projectile, target, ReactionType{ 875 } };
        auto mt876 = ReactionID{ projectile, target, ReactionType{ 876 } };
        auto mt877 = ReactionID{ projectile, target, ReactionType{ 877 } };
        std::vector< double > refXS = sumRanges( 
            reactions.at( mt875 ).template crossSections< RPair >().second, 
            reactions.at( mt876 ).template crossSections< RPair >().second, 
            reactions.at( mt877 ).template crossSections< RPair >().second );

        auto reaction = summations.at( ID );
        CHECK( refXS == reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 27 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 27 } };
        auto mt18 = ReactionID{ projectile, target, ReactionType{ 18 } };
        auto mt101 = ReactionID{ projectile, target, ReactionType{ 101 } };
        std::vector< double > refXS = sumRanges( 
          reactions.at( mt18 ).template crossSections< RPair >().second, 
          summations.at( mt101 ).template crossSections< RPair >().second 
        );

        auto reaction = summations.at( ID );
        CHECK( refXS == reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 101 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 101 } };
        auto mt102 = ReactionID{ projectile, target, ReactionType{ 102 } };
        std::vector< double > refXS = sumRanges( 
          reactions.at( mt102 ).template crossSections< RPair >().second 
        );

        auto reaction = summations.at( ID );
        CHECK( refXS == reaction.crossSections< RPair >().second );
      } // THEN
    } // WHEN
  } // GIVEN
  GIVEN( "an SLBW object" ){
    auto [energies, r2d2] = lin_recon( "SLBW", absTol, relTol );
    auto sizeEnergies = ranges::distance( energies );

    WHEN( "cross sections have been summated" ){
      tRECONR::summateReactions( logger, r2d2, energies );

      auto projectile = r2d2.projectile();
      auto target = r2d2.target();

      auto reactions = r2d2.reactions();
      auto summations = r2d2.summations();

      THEN( "the reactions can be summed up and checked" ){
        std::vector< njoy::RECONR::ReactionID > refReactions{ 
          ReactionID{ projectile, target, ReactionType{ 1 } },
          ReactionID{ projectile, target, ReactionType{ 2 } },
          ReactionID{ projectile, target, ReactionType{ 18 } },
          ReactionID{ projectile, target, ReactionType{ 51 } },
          ReactionID{ projectile, target, ReactionType{ 52 } },
          ReactionID{ projectile, target, ReactionType{ 102 } },
          ReactionID{ projectile, target, ReactionType{ 875 } },
          ReactionID{ projectile, target, ReactionType{ 876 } },
          ReactionID{ projectile, target, ReactionType{ 877 } }
        };
        auto rxKeys = ranges::view::keys( reactions ) | ranges::to_vector;
        std::sort( refReactions.begin(), refReactions.end() );
        std::sort( rxKeys.begin(), rxKeys.end() );

        CHECK( ranges::equal( refReactions, rxKeys ) );

        std::vector< njoy::RECONR::ReactionID > refSummations{ 
          ReactionID{ projectile, target, ReactionType{ 1 } },
          ReactionID{ projectile, target, ReactionType{ 4 } },
          ReactionID{ projectile, target, ReactionType{ 16 } },
          ReactionID{ projectile, target, ReactionType{ 27 } },
          ReactionID{ projectile, target, ReactionType{ 101 } }
        };
        auto smKeys = ranges::view::keys( summations ) | ranges::to_vector;
        std::sort( refSummations.begin(), refSummations.end() );
        std::sort( smKeys.begin(), smKeys.end() );

        CHECK( ranges::equal( refSummations, smKeys ) );

      } // THEN
      THEN( "MT = 1 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 1 } };

        auto mt2 = ReactionID{ projectile, target, ReactionType{ 2 } };
        auto mt4 = ReactionID{ projectile, target, ReactionType{ 4 } };
        auto mt16 = ReactionID{ projectile, target, ReactionType{ 16 } };
        auto mt27 = ReactionID{ projectile, target, ReactionType{ 27 } };
        std::vector< double  > refXS = sumRanges(
          reactions.at(  mt2   ).template crossSections< RPair >().second,
          summations.at( mt4   ).template crossSections< RPair >().second,
          summations.at( mt16  ).template crossSections< RPair >().second,
          summations.at( mt27  ).template crossSections< RPair >().second
        );

        auto reaction = summations.at( ID );
        details::checkRanges( refXS, reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 4 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 4 } };
        auto mt51 = ReactionID{ projectile, target, ReactionType{ 51 } };
        auto mt52 = ReactionID{ projectile, target, ReactionType{ 52 } };
        std::vector< double > refXS = sumRanges( 
          reactions.at( mt51 ).template crossSections< RPair >().second, 
          reactions.at( mt52 ).template crossSections< RPair >().second
        );

        auto reaction = summations.at( ID );
        CHECK( refXS == reaction.crossSections< RPair >().second );

      } // THEN
      THEN( "MT = 16 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 16 } };
        auto mt875 = ReactionID{ projectile, target, ReactionType{ 875 } };
        auto mt876 = ReactionID{ projectile, target, ReactionType{ 876 } };
        auto mt877 = ReactionID{ projectile, target, ReactionType{ 877 } };
        std::vector< double > refXS = sumRanges( 
            reactions.at( mt875 ).template crossSections< RPair >().second, 
            reactions.at( mt876 ).template crossSections< RPair >().second, 
            reactions.at( mt877 ).template crossSections< RPair >().second );

        auto reaction = summations.at( ID );
        CHECK( refXS == reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 27 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 27 } };
        auto mt18 = ReactionID{ projectile, target, ReactionType{ 18 } };
        auto mt101 = ReactionID{ projectile, target, ReactionType{ 101 } };
        std::vector< double > refXS = sumRanges( 
          reactions.at( mt18 ).template crossSections< RPair >().second, 
          summations.at( mt101 ).template crossSections< RPair >().second 
        );

        auto reaction = summations.at( ID );
        CHECK( refXS == reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 101 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 101 } };
        auto mt102 = ReactionID{ projectile, target, ReactionType{ 102 } };
        std::vector< double > refXS = sumRanges( 
          reactions.at( mt102 ).template crossSections< RPair >().second 
        );

        auto reaction = summations.at( ID );
        CHECK( refXS == reaction.crossSections< RPair >().second );
      } // THEN
    } // WHEN
    WHEN( "productions have been linearized and summarized" ){
      
      tRECONR::summateProductions( logger, r2d2, energies );

      auto& productions = r2d2.photonProductions();

      auto projectile = r2d2.projectile();
      auto target = r2d2.target();

      THEN( "the productions can be summed up and checked" ){
        std::vector< njoy::RECONR::ReactionID > reference{
          ReactionID{ projectile, target, ReactionType{ 3 } },
          ReactionID{ projectile, target, ReactionType{ 18 } },
        };
        auto keys = ranges::view::keys( productions ) | ranges::to_vector;
        std::sort( reference.begin(), reference.end() );
        std::sort( keys.begin(), keys.end() );

        CHECK( ranges::equal( reference, keys ) );
        THEN( "MT = 3 can be tested" ){
          auto ID = ReactionID{ projectile, target, ReactionType{ 3 } };
          std::vector< double > refP{
            0,      2.3,      3.3,      4.3,      
            5.3,   6.3,      7.3,      8.3,      9.3,      
            10.3,  11.1276,  11.2138,  
            11.3,  0
            };

          auto production = productions.at( ID ).productions< PPair >()
            | ranges::view::transform( []( auto&& p ){ return p.second; } )
            | ranges::to_vector;

          details::checkRanges( refP, production );
        } // THEN
        THEN( "MT = 18 can be tested" ){
          auto ID = ReactionID{ projectile, target, ReactionType{ 18 } };
          std::vector< double > refP{
            0,        10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.1801,  
            10.1802,  10.1804,  10.1805,  10.1806,  10.1807,  
            10.1808,  10.1809,  10.181,   10.1812,  10.1814,  
            10.1816,  10.1817,  10.1821,  10.1825,  10.1829,  
            10.1831,  10.1833,  10.1842,  10.1846,  10.1848,  
            10.1849,  10.1849,  10.185,   10.185,   10.185,   
            10.185,   10.185,   10.185,   10.185,   10.185,   
            10.185,   10.185,   10.185,   10.185,   10.185,   
            10.185,   10.185,   10.185,   10.185,   10.185,   
            10.185,   10.185,   10.185,   10.1862,  10.1925,  
            10.195,   10.1987,  10.2,     10.2019,  10.2025,  
            10.2034,  10.2037,  10.2042,  10.2044,  10.2046,  
            10.2047,  10.2048,  10.2048,  10.2049,  10.2049,  
            10.205,   10.205,   10.205,   10.205,   10.205,   
            10.205,   10.205,   10.205,   10.205,   10.205,   
            10.205,   10.205,   10.205,   10.205,   10.205,   
            10.205,   10.205,   10.205,   10.205,   10.205,   
            10.205,   10.205,   10.205,   10.205,   10.205,   
            10.205,   10.205,   10.205,   10.205,   10.205,   
            10.205,   10.205,   10.205,   10.205,   10.205,   
            10.205,   10.205,   10.205,   10.205,   10.205,   
            10.205,   10.2163,  10.2467,  10.3467,  10.3633,  
            10.38,    10.3967,  10.4133,  10.43,    10.4467,  
            10.6533,  10.8133,  10.83,    10.8467,  11.18
        };

          auto production = productions.at( ID ).productions< PPair >()
            | ranges::view::transform( []( auto&& p ){ return p.second; } )
            | ranges::to_vector;
          details::checkRanges( refP, production );
        } // THEN
      } // THEN
    } // WHEN
  } // GIVEN
  GIVEN( "an RM object" ){
    auto [energies, r2d2] = lin_recon( "RM", absTol, relTol );
    auto sizeEnergies = ranges::distance( energies );

    WHEN( "cross sections have been summated" ){
      tRECONR::summateReactions( logger, r2d2, energies );

      auto projectile = r2d2.projectile();
      auto target = r2d2.target();

      auto reactions = r2d2.reactions();
      auto summations = r2d2.summations();

      THEN( "the reactions can be summed up and checked" ){
        std::vector< njoy::RECONR::ReactionID > refReactions{ 
          ReactionID{ projectile, target, ReactionType{ 1 } },
          ReactionID{ projectile, target, ReactionType{ 2 } },
          ReactionID{ projectile, target, ReactionType{ 18 } },
          ReactionID{ projectile, target, ReactionType{ 51 } },
          ReactionID{ projectile, target, ReactionType{ 52 } },
          ReactionID{ projectile, target, ReactionType{ 102 } },
          ReactionID{ projectile, target, ReactionType{ 875 } },
          ReactionID{ projectile, target, ReactionType{ 876 } },
          ReactionID{ projectile, target, ReactionType{ 877 } }
        };
        auto rxKeys = ranges::view::keys( reactions ) | ranges::to_vector;
        std::sort( refReactions.begin(), refReactions.end() );
        std::sort( rxKeys.begin(), rxKeys.end() );

        CHECK( ranges::equal( refReactions, rxKeys ) );

        std::vector< njoy::RECONR::ReactionID > refSummations{ 
          ReactionID{ projectile, target, ReactionType{ 1 } },
          ReactionID{ projectile, target, ReactionType{ 4 } },
          ReactionID{ projectile, target, ReactionType{ 16 } },
          ReactionID{ projectile, target, ReactionType{ 27 } },
          ReactionID{ projectile, target, ReactionType{ 101 } }
        };
        auto smKeys = ranges::view::keys( summations ) | ranges::to_vector;
        std::sort( refSummations.begin(), refSummations.end() );
        std::sort( smKeys.begin(), smKeys.end() );

        CHECK( ranges::equal( refSummations, smKeys ) );

        printKeys( refSummations, smKeys );
      } // THEN
      THEN( "MT = 1 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 1 } };

        auto mt2 = ReactionID{ projectile, target, ReactionType{ 2 } };
        auto mt4 = ReactionID{ projectile, target, ReactionType{ 4 } };
        auto mt16 = ReactionID{ projectile, target, ReactionType{ 16 } };
        auto mt27 = ReactionID{ projectile, target, ReactionType{ 27 } };
        // Don't need 101 because it is included in 27

        std::vector< double  > refXS = sumRanges(
          reactions.at(  mt2   ).template crossSections< RPair >().second,
          summations.at( mt4   ).template crossSections< RPair >().second,
          summations.at( mt16  ).template crossSections< RPair >().second,
          summations.at( mt27  ).template crossSections< RPair >().second
        );

        auto reaction = summations.at( ID );
        details::checkRanges( refXS, reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 4 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 4 } };
        auto mt51 = ReactionID{ projectile, target, ReactionType{ 51 } };
        auto mt52 = ReactionID{ projectile, target, ReactionType{ 52 } };
        std::vector< double > refXS = sumRanges( 
            reactions.at( mt51 ).template crossSections< RPair >().second, 
            reactions.at( mt52 ).template crossSections< RPair >().second );

        auto reaction = summations.at( ID );
        CHECK( refXS == reaction.crossSections< RPair >().second );

      } // THEN
      THEN( "MT = 16 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 16 } };
        auto mt875 = ReactionID{ projectile, target, ReactionType{ 875 } };
        auto mt876 = ReactionID{ projectile, target, ReactionType{ 876 } };
        auto mt877 = ReactionID{ projectile, target, ReactionType{ 877 } };
        std::vector< double > refXS = sumRanges( 
            reactions.at( mt875 ).template crossSections< RPair >().second, 
            reactions.at( mt876 ).template crossSections< RPair >().second, 
            reactions.at( mt877 ).template crossSections< RPair >().second );

        auto reaction = summations.at( ID );
        CHECK( refXS == reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 27 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 27 } };
        auto mt18 = ReactionID{ projectile, target, ReactionType{ 18 } };
        auto mt101 = ReactionID{ projectile, target, ReactionType{ 101 } };
        std::vector< double > refXS = sumRanges( 
          reactions.at( mt18 ).template crossSections< RPair >().second, 
          summations.at( mt101 ).template crossSections< RPair >().second 
        );

        auto reaction = summations.at( ID );
        CHECK( refXS == reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 101 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 101 } };
        auto mt102 = ReactionID{ projectile, target, ReactionType{ 102 } };
        std::vector< double > refXS = sumRanges( 
          reactions.at( mt102 ).template crossSections< RPair >().second 
        );

        auto reaction = summations.at( ID );
        CHECK( refXS == reaction.crossSections< RPair >().second );
      } // THEN
    } // WHEN
  } // GIVEN
  GIVEN( "an RML object" ){
    auto [energies, r2d2] = lin_recon( "RML", absTol, relTol );
    auto sizeEnergies = ranges::distance( energies );

    WHEN( "cross sections have been summated" ){
      tRECONR::summateReactions( logger, r2d2, energies );

      auto projectile = r2d2.projectile();
      auto target = r2d2.target();

      auto reactions = r2d2.reactions();
      auto summations = r2d2.summations();

      THEN( "the reactions can be summed up and checked" ){
        std::vector< njoy::RECONR::ReactionID > refReactions{ 
          ReactionID{ projectile, target, ReactionType{ 1 } },
          ReactionID{ projectile, target, ReactionType{ 2 } },
          ReactionID{ projectile, target, ReactionType{ 18 } },
          ReactionID{ projectile, target, ReactionType{ 51 } },
          ReactionID{ projectile, target, ReactionType{ 52 } },
          ReactionID{ projectile, target, ReactionType{ 102 } },
          ReactionID{ projectile, target, ReactionType{ 875 } },
          ReactionID{ projectile, target, ReactionType{ 876 } },
          ReactionID{ projectile, target, ReactionType{ 877 } }
        };
        auto rxKeys = ranges::view::keys( reactions ) | ranges::to_vector;
        std::sort( refReactions.begin(), refReactions.end() );
        std::sort( rxKeys.begin(), rxKeys.end() );

        CHECK( ranges::equal( refReactions, rxKeys ) );

        std::vector< njoy::RECONR::ReactionID > refSummations{ 
          ReactionID{ projectile, target, ReactionType{ 1 } },
          ReactionID{ projectile, target, ReactionType{ 4 } },
          ReactionID{ projectile, target, ReactionType{ 16 } },
          ReactionID{ projectile, target, ReactionType{ 27 } },
          ReactionID{ projectile, target, ReactionType{ 101 } }
        };
        auto smKeys = ranges::view::keys( summations ) | ranges::to_vector;
        std::sort( refSummations.begin(), refSummations.end() );
        std::sort( smKeys.begin(), smKeys.end() );

        CHECK( ranges::equal( refSummations, smKeys ) );

        printKeys( refSummations, smKeys );
      } // THEN
      THEN( "MT = 1 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 1 } };

        auto mt2 = ReactionID{ projectile, target, ReactionType{ 2 } };
        auto mt4 = ReactionID{ projectile, target, ReactionType{ 4 } };
        auto mt16 = ReactionID{ projectile, target, ReactionType{ 16 } };
        auto mt27 = ReactionID{ projectile, target, ReactionType{ 27 } };
        // Don't need 101 because it is included in 27

        std::vector< double  > refXS = sumRanges(
          reactions.at(  mt2   ).template crossSections< RPair >().second,
          summations.at( mt4   ).template crossSections< RPair >().second,
          summations.at( mt16  ).template crossSections< RPair >().second,
          summations.at( mt27  ).template crossSections< RPair >().second
        );

        auto reaction = summations.at( ID );
        details::checkRanges( refXS, reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 4 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 4 } };
        auto mt51 = ReactionID{ projectile, target, ReactionType{ 51 } };
        auto mt52 = ReactionID{ projectile, target, ReactionType{ 52 } };
        std::vector< double > refXS = sumRanges( 
            reactions.at( mt51 ).template crossSections< RPair >().second, 
            reactions.at( mt52 ).template crossSections< RPair >().second );

        auto reaction = summations.at( ID );
        CHECK( refXS == reaction.crossSections< RPair >().second );

      } // THEN
      THEN( "MT = 16 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 16 } };
        auto mt875 = ReactionID{ projectile, target, ReactionType{ 875 } };
        auto mt876 = ReactionID{ projectile, target, ReactionType{ 876 } };
        auto mt877 = ReactionID{ projectile, target, ReactionType{ 877 } };
        std::vector< double > refXS = sumRanges( 
            reactions.at( mt875 ).template crossSections< RPair >().second, 
            reactions.at( mt876 ).template crossSections< RPair >().second, 
            reactions.at( mt877 ).template crossSections< RPair >().second );

        auto reaction = summations.at( ID );
        CHECK( refXS == reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 27 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 27 } };
        auto mt18 = ReactionID{ projectile, target, ReactionType{ 18 } };
        auto mt101 = ReactionID{ projectile, target, ReactionType{ 101 } };
        std::vector< double > refXS = sumRanges( 
          reactions.at( mt18 ).template crossSections< RPair >().second, 
          summations.at( mt101 ).template crossSections< RPair >().second 
        );

        auto reaction = summations.at( ID );
        CHECK( refXS == reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 101 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 101 } };
        auto mt102 = ReactionID{ projectile, target, ReactionType{ 102 } };
        std::vector< double > refXS = sumRanges( 
          reactions.at( mt102 ).template crossSections< RPair >().second 
        );

        auto reaction = summations.at( ID );
        CHECK( refXS == reaction.crossSections< RPair >().second );
      } // THEN
    } // WHEN
  } // GIVEN
} // SCENARIO

