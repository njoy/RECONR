SCENARIO( "Testing the summation of cross sections" ){
  double absTol{ 1E-6 };
  double relTol{ 1E-1 }; // This tolerance is large by design

  using RPair = njoy::RECONR::XSPair;
  using PPair = njoy::RECONR::PPair;

  GIVEN( "an SLBW object" ){
    auto [energies, r2d2] = lin_recon( "SLBW", absTol, relTol );
    auto sizeEnergies = ranges::distance( energies );

    WHEN( "cross sections have been summated" ){
      tRECONR::summateReactions( std::cout, std::cout, r2d2, energies );

      auto projectile = r2d2.projectile();
      auto target = r2d2.target();

      auto reactions = r2d2.reactions();
      auto summations = r2d2.summations();

      THEN( "the reactions can be summed up and checked" ){
        std::vector< njoy::RECONR::ReactionID > refReactions{ 
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
      THEN( "MT = 2 can be tested" ){ 
        auto capture = ReactionID{ projectile, target, ReactionType{ 2 } };
        std::vector< double > refXS{
          2.12506,  2.12506,  2.12506,  2.12506,  2.12506,  
          2.12506,  2.12506,  2.12506,  2.12506,  2.12506,  
          2.12506,  2.12506,  2.12506,  2.12506,  2.12506,  
          2.12506,  2.12506,  2.12506,  2.12506,  2.12505,  
          2.12504,  2.12503,  2.12499,  2.12493,  2.12483,  
          2.12469,  2.12469,  2.12577,  2.12625,  2.13254,  
          2.17054,  2.27584,  2.39337,  2.6277,   2.99912,  
          3.85593,  4.77109,  6.54644,  10.5524,  14.5282,  
          21.0871,  28.9515,  40.1473,  21.0819,  12.8938,  
          8.71798,  6.47505,  5.16996,  3.81534,  3.17209,  
          2.60656,  2.37352,  2.21458,  2.14388,  2.08834,  
          2.07845,  2.09168,  2.11455,  2.16429,  2.28823,  
          2.42846,  2.71949,  3.05579,  3.69336,  5.07632,  
          8.48055,  11.1456,  14.9402,  8.47874,  5.88827,  
          4.48489,  3.22304,  2.71756,  2.47115,  2.24985,  
          2.15672,  2.1089,   2.08104,  2.05131,  2.03639,  
          2.02772,  2.0183,   2.01043,  2.00704,  2.00472,  
          2.00344,  2.00344,  2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        0
        };

        auto reaction = reactions.at( capture );
        details::checkRanges( refXS, reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 4 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 4 } };
        auto mt51 = ReactionID{ projectile, target, ReactionType{ 51 } };
        auto mt52 = ReactionID{ projectile, target, ReactionType{ 52 } };
        std::vector< double > refXS = sumRanges( 
            reactions.at( mt51 ).template crossSections< RPair >().second, 
            reactions.at( mt52 ).template crossSections< RPair >().second );
        zeroXS( energies, refXS, 1E5, 2E7 );

        auto reaction = summations.at( ID );
        CHECK( refXS == reaction.crossSections< RPair >().second );

      } // THEN
      THEN( "MT = 16 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 16 } };
        std::vector< double > refXS( sizeEnergies, 1.5 );
        zeroXS( energies, refXS, 1E5, 2E7 );

        auto reaction = summations.at( ID );
        CHECK( refXS == reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 18 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 18 } };
        std::vector< double > refXS{
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            18,           18,           11.829,       
          11.829,       11.829,       11.829,       3.34739e-05,  3.3363e-05,   
          3.3066e-05,   3.26663e-05,  3.20868e-05,  3.19236e-05,  3.17605e-05,  
          3.15973e-05,  3.14341e-05,  3.12709e-05,  3.11077e-05,  2.90842e-05,  
          2.75176e-05,  2.75176e-05,  2.7313e-05,   2.7313e-05,   2.71079e-05,  
          2.71079e-05,  0
        };

        auto reaction = reactions.at( ID );
        details::checkRanges( refXS, reaction.crossSections< RPair >().second );

      } // THEN
      THEN( "MT = 51 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 51 } };
        std::vector< double > refXS( sizeEnergies, 51.0 );
        zeroXS( energies, refXS, 1E5, 2E7 );

        auto reaction = reactions.at( ID );
        details::checkRanges( refXS, reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 52 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 52 } };
        std::vector< double > refXS( sizeEnergies, 52.0 );
        zeroXS( energies, refXS, 1E5, 2E7 );

        auto reaction = reactions.at( ID );
        details::checkRanges( refXS, reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 102 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 102 } };
        std::vector< double > refXS{
          6952.24,      6739.82,      6545.15,      6365.86,      6200.01,      
          6046,         5902.48,      5768.28,      5642.45,      5524.13,      
          5412.61,      5207.49,      5022.93,      4855.67,      4855.67,      
          3360.98,      2726.35,      2102.12,      1563.45,      1137.67,      
          818.15,       585.098,      417.606,      298.116,      213.173,      
          152.912,      110.329,      80.7439,      77.0078,      61.8575,      
          56.5561,      71.9926,      92.5391,      134.368,      200.545,      
          351.623,      511.454,      819.044,      1506.89,      2184.75,      
          3296.11,      4618.73,      6453.3,       3171.71,      1794.73,      
          1098.14,      726.554,      511.711,      290.587,      186.802,      
          96.9413,      60.7023,      36.624,       26.2712,      18.6415,      
          17.7697,      20.6723,      24.8943,      33.7429,      55.3442,      
          79.5513,      129.501,      186.994,      295.659,      530.621,      
          1106.91,      1556.59,      2191.32,      1092.5,       655.408,      
          419.217,      207.478,      123.004,      81.9893,      45.3585,      
          30.0722,      22.2935,      17.8033,      13.0743,      10.7481,      
          9.4208,       8.01459,      6.873,        6.37959,      6.06902,      
          5.88624,      5.88624,      5.38812,      5.05137,      4.71461,      
          4.37786,      4.0411,       3.7848,       3.70435,      3.36759,      
          3.03083,      2.69408,      2.35732,      2.02057,      1.68381,      
          1.34706,      1.0103,       0.98785,      0.808247,     0.808247,     
          0.673545,     0.336789,     3.34739e-05,  3.34739e-05,  3.3363e-05,   
          3.3066e-05,   3.26663e-05,  3.20868e-05,  3.19236e-05,  3.17605e-05,  
          3.15973e-05,  3.14341e-05,  3.12709e-05,  3.11077e-05,  2.90842e-05,  
          2.75176e-05,  2.75176e-05,  2.7313e-05,   2.7313e-05,   2.71079e-05,  
          2.71079e-05,  0
        };
        auto reaction = reactions.at( ID );
        details::checkRanges( refXS, reaction.crossSections< RPair >().second );
      } // THEN
    } // WHEN
    WHEN( "productions have been linearized and summarized" ){
      
      tRECONR::summateProductions( std::cout, std::cout, r2d2, energies );

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
            0.0, 2.3, 2.70822, 3.3, 4.3, 5.3, 6.3, 7.3, 8.3, 9.3, 10.3, 
            11.1276, 11.1276, 11.2138, 11.2138, 11.3, 11.3, 0.0
            };

          auto production = productions.at( ID ).productions< PPair >()
            | ranges::view::transform( []( auto&& p ){ return p.second; } )
            | ranges::to_vector;
          details::checkRanges( refP, production );
        } // THEN
        THEN( "MT = 18 can be tested" ){
          auto ID = ReactionID{ projectile, target, ReactionType{ 18 } };
          std::vector< double > refP{
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
            10.18,    10.1801,  10.1802,  10.1804,  10.1808,  
            10.1816,  10.1831,  10.1833,  10.185,   10.185,   
            10.1863,  10.1925,  10.205,   10.205,   10.2163,  
            10.2467,  10.2875,  10.3467,  10.3633,  10.38,    
            10.3967,  10.4133,  10.43,    10.4467,  10.6533,  
            10.8133,  10.8133,  10.83,    10.83,    10.8467,  
            10.8467,  11.18
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
      tRECONR::summateReactions( std::cout, std::cout, r2d2, energies );

      auto projectile = r2d2.projectile();
      auto target = r2d2.target();

      auto reactions = r2d2.reactions();
      auto summations = r2d2.summations();

      THEN( "the reactions can be summed up and checked" ){
        std::vector< njoy::RECONR::ReactionID > refReactions{ 
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
      THEN( "MT = 2 can be tested" ){
        auto capture = ReactionID{ projectile, target, ReactionType{ 2 } };
        std::vector< double > refXS{
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        16.5826,  16.5634,  16.5131,  16.8464,  
          17.02,    16.8912,  16.7787,  16.6211,  16.4759,  
          16.3538,  16.2227,  16.18,    16.1255,  16.0463,  
          15.99,    15.924,   15.9353,  16.2386,  16.5252,  
          16.5097,  16.4214,  16.3556,  16.3078,  16.241,   
          16.1931,  16.1203,  16.0599,  15.9471,  15.8813,  
          15.8362,  15.7759,  15.7421,  15.7296,  15.7194,  
          15.6767,  15.608,   15.5652,  15.5287,  15.6968,  
          15.7725,  15.8435,  15.9331,  15.8284,  15.7176,  
          15.6097,  15.4893,  15.3362,  15.2434,  15.156,   
          15.1662,  15.6013,  16.2867,  16.7619,  16.821,   
          16.7118,  16.5741,  16.4504,  16.2624,  16.1328,  
          15.9657,  15.8571,  15.7056,  15.5815,  15.4505,  
          15.2714,  15.1313,  14.8992,  14.7039,  14.3859,  
          14.1396,  13.7954,  13.3533,  13.1917,  13.3624,  
          16.4855,  18.9067,  18.5401,  17.9962,  17.5562,  
          17.2252,  16.7809,  16.5025,  16.1737,  15.9824,  
          15.7587,  15.6287,  15.5656,  15.5755,  15.5755,  
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          0
        };

        auto reaction = reactions.at( capture );
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
        zeroXS( energies, refXS, 1E5, 2E7 );

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
      THEN( "MT = 51 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 51 } };
        std::vector< double > refXS( sizeEnergies, 51.0 );
        zeroXS( energies, refXS, 1E5, 2E7 );

        auto reaction = reactions.at( ID );
        details::checkRanges( refXS, reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 52 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 52 } };
        std::vector< double > refXS( sizeEnergies, 52.0 );
        zeroXS( energies, refXS, 1E5, 2E7 );

        auto reaction = reactions.at( ID );
        details::checkRanges( refXS, reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 102 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 102 } };
        std::vector< double > refXS{
          102,          94.1134,      86.6777,      79.6441,      72.9714,      
          66.6243,      60.5726,      54.7899,      49.2533,      43.9428,      
          38.8406,      29.2,         20.2247,      11.829,       11.829,       
          11.2665,      11.0128,      10.7235,      10.4124,      10.089,       
          9.75904,      9.42573,      9.0907,       8.75481,      8.41849,      
          8.08195,      7.7453,       7.70549,      7.4086,       7.07187,      
          6.73513,      16.5412,      17.5337,      26.2096,      42.6957,      
          25.6779,      16.9795,      13.1785,      10.3098,      9.14646,      
          8.86177,      9.6696,       10.4949,      12.2795,      17.0288,      
          22.8138,      35.3477,      71.3244,      95.1092,      70.6882,      
          34.002,       20.3416,      14.8569,      12.2275,      9.91456,      
          8.96154,      8.20573,      7.93812,      7.96811,      8.46485,      
          9.14011,      10.2678,      10.7113,      10.404,       9.45222,      
          9.45907,      10.7438,      12.3269,      15.2668,      26.1406,      
          26.6563,      26.1529,      17.7846,      12.9288,      11.3829,      
          11.4133,      12.8691,      17.1729,      21.9717,      31.0661,      
          48.9715,      77.0302,      85.8931,      70.871,       50.9405,      
          36.3481,      27.1344,      21.3648,      15.1295,      12.1192,      
          9.48253,      8.42001,      7.65081,      7.51666,      7.7913,       
          8.89895,      10.4355,      14.451,       19.4155,      31.1338,      
          44.0079,      69.8322,      131.042,      194.129,      298.6,        
          505.502,      307.189,      177.23,       109.438,      73.6256,      
          53.2245,      32.5834,      23.059,       14.9262,      11.7052,      
          9.36352,      8.7692,       8.87186,      8.7676,       8.7676,       
          5.72488,      5.38812,      5.05137,      4.71461,      4.37786,      
          4.0411,       3.7848,       3.70435,      3.36759,      3.03083,      
          2.69408,      2.35732,      2.02057,      1.68381,      1.34706,      
          1.0103,       0.98785,      0.808247,     0.808247,     0.673545,     
          0.336789,     3.34739e-05,  3.34739e-05,  3.3363e-05,   3.3066e-05,   
          3.26663e-05,  3.20868e-05,  3.19236e-05,  3.17605e-05,  3.15973e-05,  
          3.14341e-05,  3.12709e-05,  3.11077e-05,  2.90842e-05,  2.75176e-05,  
          2.75176e-05,  2.7313e-05,   2.7313e-05,   2.71079e-05,  2.71079e-05,  
          0
        };
        auto reaction = reactions.at( ID );
        details::checkRanges( refXS, reaction.crossSections< RPair >().second );
      } // THEN
    } // WHEN
  } // GIVEN
  GIVEN( "an RML object" ){
    auto [energies, r2d2] = lin_recon( "RML", absTol, relTol );
    auto sizeEnergies = ranges::distance( energies );

    WHEN( "cross sections have been summated" ){
      tRECONR::summateReactions( std::cout, std::cout, r2d2, energies );

      auto projectile = r2d2.projectile();
      auto target = r2d2.target();

      auto reactions = r2d2.reactions();
      auto summations = r2d2.summations();

      THEN( "the reactions can be summed up and checked" ){
        std::vector< njoy::RECONR::ReactionID > refReactions{ 
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
        // zeroXS( energies, refXS, 1E5, 2E7 );

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
          reactions.at( mt877 ).template crossSections< RPair >().second
        );

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

