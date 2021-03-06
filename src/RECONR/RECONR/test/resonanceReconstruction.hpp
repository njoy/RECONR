SCENARIO( "Testing the resonance reconstruction" ){
  GIVEN( "an SLBW R2D2 object and reference grid" ){
    auto material = details::ENDFMaterial( "SLBW", true );
    auto r2d2 = njoy::RECONR::R2D2::Factory()( logger, material );

    auto proj = r2d2.projectile();
    auto target = r2d2.target();

    auto eID = ReactionID{ proj, target, ReactionType{ 2 } };
    auto fID = ReactionID{ proj, target, ReactionType{ 19 } };
    auto cID = ReactionID{ proj, target, ReactionType{ 102 } };

    const std::vector< double > refGrid{
      0.00001, 0.9860692, 1.0253, 1.0645308, 2.9860692, 3.0253, 3.0645308, 7.5
    };

    WHEN( "the resonances are reconstructed" ){
      tRECONR::reconstructResonances( logger, refGrid, r2d2, 1E-1, 1E-3 );

      THEN( "the linearized reconstruction can be verified" ){
        auto reconstructed = r2d2.reconstructedResonances();

        auto keys = ranges::view::keys( reconstructed );

        std::vector< double > refEnergies{ 
          1e-05, 2.50461e-05, 4.00921e-05, 7.01843e-05, 0.000130369, 0.000250737,
          0.000491474, 0.000972948, 0.0019359, 0.00386179, 0.00771359, 0.0154172,
          0.0308244, 0.0616387, 0.123267, 0.246525, 0.49304, 0.616297,
          0.739554, 0.801183, 0.862812, 0.893626, 0.92444, 0.955255,
          0.986069, 1.0253, 1.06453, 1.09455, 1.12458, 1.1546,
          1.18463, 1.24468, 1.30472, 1.42482, 1.54492, 1.66501,
          1.78511, 2.0253, 2.26549, 2.50568, 2.62578, 2.74588,
          2.80592, 2.86597, 2.896, 2.92602, 2.95605, 2.98607,
          3.0253, 3.06453, 3.08186, 3.09918, 3.13384, 3.16849,
          3.20314, 3.27244, 3.34175, 3.48036, 3.61896, 3.75757,
          3.89618, 4.1734, 4.72783, 5.28227, 6.39113, 7.5
        };
        std::vector< double > refElastic{
          0.12506,   0.125059,  0.125059,  0.125058,   0.125057,   0.125054,    
          0.125048,  0.125036,  0.125014,  0.124971,   0.124893,   0.124769,    
          0.124651,  0.124929,  0.127583,  0.142374,   0.239419,   0.376941,    
          0.731633,  1.15853,   2.12402,   3.132,      5.03349,    9.1272,      
          19.0871,   38.1473,   19.0819,   9.28208,    5.16956,    3.23402,     
          2.19952,   1.20137,   0.756675,  0.38362,    0.235867,   0.163379,    
          0.12328,   0.0861085, 0.0790813, 0.10576,    0.151698,   0.274666,    
          0.423839,  0.760705,  1.11065,   1.76604,    3.15938,    6.48055,     
          12.9402,   6.47874,   4.21545,   2.85963,    1.51044,    0.9191,      
          0.616025,  0.333129,  0.21033,   0.108652,   0.0682899,  0.0480401,   
          0.0363148, 0.0237135, 0.0133756, 0.00901362, 0.00516115, 0.00343646
        };
        std::vector< double > refCapture{ 
          6850.24,  4328.48,  3421.17,  2585.72,  1897.19,   1367.98,   
          977.059,  694.372,  492.186,  348.377,  246.372,   174.131,   
          123.081,  87.2921,  63.1015,  49.8161,  58.9968,   82.7983,   
          146.211,  222.108,  391.906,  567.549,  896.41,    1598.51,   
          3289.5,   6446.72,  3165.14,  1518.74,  834.808,   515.696,   
          346.513,  185.012,  114.139,  55.8541,  33.4273,   22.7445,   
          17.0292,  12.114,   11.835,   17.1509,  25.4053,   46.9056,   
          72.6816,  130.496,  190.301,  301.974,  538.661,   1100.86,   
          2185.28,  1086.47,  704.507,  476.198,  249.594,   150.619,   
          100.063,  53.0942,  32.8511,  16.2643,  9.79166,   6.60478,   
          4.79638,  2.91043,  1.45495,  0.89217,  0.443622,  0.265993
        };
        std::vector< double > refFission( 66, 0.0 );

        auto elastic = reconstructed.at( eID ).front();
        auto capture = reconstructed.at( cID ).front();
        auto fission = reconstructed.at( fID ).front();
        details::checkRanges( refEnergies, elastic.x() );
        details::checkRanges( refElastic, elastic.y() );
        details::checkRanges( refEnergies, capture.x() );
        details::checkRanges( refCapture, capture.y() );
        details::checkRanges( refEnergies, fission.x() );
        details::checkRanges( refFission, fission.y() );
        
      } // THEN
      THEN( "the unresolved resonance keys can also be verified" ){
        const auto& unresolved = r2d2.unresolved();

        std::vector< ReactionID > refKeys{ eID, cID };
        auto keys = ranges::view::keys( unresolved ) | ranges::to_vector;

        std::sort( refKeys.begin(), refKeys.end() );
        std::sort( keys.begin(), keys.end() );
        CHECK( ranges::equal( refKeys, keys ) );

      } // THEN
      THEN( "the unresolved capture cross section can be verified" ){
        const auto& unresolved = r2d2.unresolved();

        std::vector< double > refEnergies{
          15000,  17000,  20000,   25000,  30000,  
          35000,  40000,  50000,   60000,  70000,  
          80000,  90000,  100000
        };
        std::vector< double > refXS{
          0.00500132,   0.00442008,  0.00376465,   0.00301958,   
          0.00252133,   0.00216459,  0.0018965,    0.00152033,   
          0.00126893,   0.001089,    0.000953847,  0.000848589,  
          0.000764289
        };
        auto capture = unresolved.at( cID );
        auto crossSections = capture.crossSections< 
            njoy::RECONR::interp::LinearLinear >();

        details::checkRanges( refEnergies, crossSections.x() );
        details::checkRanges( refXS, crossSections.y() );
      } // THEN
      THEN( "the unresolved elastic cross section can be verified" ){
        const auto& unresolved = r2d2.unresolved();

        std::vector< double > refEnergies{
          15000,  17000,  20000,   25000,  30000,  
          35000,  40000,  50000,   60000,  70000,  
          80000,  90000,  100000
        };
        std::vector< double > refXS{
          8.73463,  8.4211,   8.03861,  7.55437,  
          7.18993,  6.90114,  6.6638,   6.29054,  
          6.00425,  5.77319,  5.57992,  5.41392,  
          5.26844
        };
        auto elastic = unresolved.at( eID );
        auto crossSections = elastic.crossSections< 
            njoy::RECONR::interp::LinearLinear >();

        details::checkRanges( refEnergies, crossSections.x() );
        details::checkRanges( refXS, crossSections.y() );
      } // THEN
      
    } // WHEN
    
  } // GIVEN
  GIVEN( "an RM R2D2 object and reference grid" ){
    auto material = details::ENDFMaterial( "RM" );
    auto r2d2 = njoy::RECONR::R2D2::Factory()( logger, material );

    auto proj = r2d2.projectile();
    auto target = r2d2.target();

    auto eID = ReactionID{ proj, target, ReactionType{ 2 } };
    auto fID = ReactionID{ proj, target, ReactionType{ 19 } };
    auto cID = ReactionID{ proj, target, ReactionType{ 102 } };

      std::vector< double > refGrid { 
        0.98596, 1.06389, 1.1389, 1.21391, 2.02109, 2.0361, 2.05111, 
        2.74827, 2.7767, 2.80513, 3.14517, 3.1566, 3.16803, 3.60111, 
        3.6208, 3.64049, 4.8336, 4.8508, 4.868, 5.24932, 5.4497, 5.5
      };

    WHEN( "the resonances are reconstructed" ){
      tRECONR::reconstructResonances( logger, refGrid, r2d2, 1E-1, 1E-3 );

      THEN( "the linearized reconstruction can be verified" ){
        auto reconstructed = r2d2.reconstructedResonances();

        auto keys = ranges::view::keys( reconstructed );

        std::vector< double > refEnergies{ 
          0.98596,  1.02493,  1.06389,  1.1389,   1.21391,  
          1.26436,  1.31481,  1.4157,   1.6175,   1.8193,   
          1.86974,  1.92019,  1.94542,  1.97064,  1.98325,  
          1.99587,  2.00848,  2.02109,  2.0361,   2.05111,  
          2.0729,   2.09468,  2.11647,  2.13826,  2.18183,  
          2.2254,   2.31255,  2.39969,  2.57398,  2.66113,  
          2.7047,   2.74827,  2.7767,   2.80513,  2.89014,  
          2.97515,  3.01765,  3.06016,  3.14517,  3.1566,   
          3.16803,  3.22216,  3.2763,   3.33044,  3.38457,  
          3.43871,  3.49284,  3.51991,  3.54697,  3.57404,  
          3.60111,  3.6208,   3.64049,  3.65913,  3.67777,  
          3.69642,  3.71506,  3.75234,  3.78963,  3.8642,   
          3.93877,  4.08791,  4.23705,  4.38618,  4.53532,  
          4.60989,  4.68446,  4.72175,  4.75903,  4.77767,  
          4.79632,  4.81496,  4.82428,  4.8336,   4.8508,   
          4.868,    4.87992,  4.89183,  4.90375,  4.91567,  
          4.9395,   4.96333,  5.01099,  5.05866,  5.15399,  
          5.24932,  5.4497,   5.5
        };
        std::vector< double > refElastic{ 
          14.5826,  14.5327,  14.5131,  14.8464,  15.0201,  
          14.8999,  14.7925,  14.6392,  14.4366,  14.2619,  
          14.2091,  14.1403,  14.0929,  14.0281,  13.9863,  
          13.939,   13.9007,  13.9354,  14.2386,  14.5251,  
          14.5097,  14.4214,  14.3556,  14.3078,  14.241,   
          14.1931,  14.1203,  14.0599,  13.9471,  13.8813,  
          13.8362,  13.7759,  13.7421,  13.7296,  13.7054,  
          13.6351,  13.5874,  13.539,   13.6968,  13.7725,  
          13.8435,  13.9331,  13.8284,  13.7176,  13.6097,  
          13.4893,  13.3362,  13.2434,  13.156,   13.1662,  
          13.6013,  14.2867,  14.7618,  14.821,   14.7118,  
          14.5741,  14.4504,  14.2624,  14.1328,  13.9657,  
          13.8571,  13.7056,  13.5815,  13.4505,  13.2714,  
          13.1313,  12.8992,  12.7039,  12.3859,  12.1396,  
          11.7954,  11.3532,  11.1917,  11.3625,  14.4855,  
          16.9067,  16.5402,  15.9963,  15.5563,  15.2253,  
          14.781,   14.5025,  14.1737,  13.9824,  13.7587,  
          13.6287,  13.5656,  13.5755
        };
        std::vector< double > refCapture{ 
          64.155,   73.7218,  89.705,   111.73,   49.7073,  
          30.5546,  23.3012,  18.4774,  15.6896,  14.3587,  
          14.2416,  14.443,   14.8764,  15.9617,  17.0766,  
          19.0255,  22.548,   28.3832,  33.3652,  26.8838,  
          18.0184,  14.7916,  13.4737,  12.8033,  12.1121,  
          11.7105,  11.1305,  10.5992,  9.04656,  7.30333,  
          6.11291,  6.59994,  9.8862,   14.2276,  19.2682,  
          22.5998,  26.3267,  33.37,    59.2035,  58.8323,  
          56.0236,  32.6866,  21.8327,  19.1471,  19.9451,  
          23.6452,  32.5672,  41.4005,  56.5703,  82.8037,  
          114.893,  112.824,  79.6278,  48.2594,  28.5979,  
          17.619,   11.493,   5.87904,  3.8114,   2.71883,  
          2.68868,  3.11531,  3.59612,  4.12546,  4.87729,  
          5.48662,  6.5506,   7.53293,  9.40118,  11.1709,  
          14.3614,  21.1263,  27.5253,  37.3192,  51.8651,  
          28.7732,  17.0467,  11.5986,  9.04432,  7.7853,   
          6.84385,  6.71141,  7.2079,   8.12153,  10.8557,  
          15.0913,  24.6041,  24.191
        };
        std::vector< double > refFission{
          64.155, 73.7218, 89.705, 111.73, 49.7073, 30.5546, 23.3012,
          18.4774, 15.6896, 14.3587, 14.2416, 14.443, 14.8764, 15.9617,
          17.0766, 19.0255, 22.548, 28.3832, 33.3652, 26.8838, 18.0184,
          14.7916, 13.4737, 12.8033, 12.1121, 11.7105, 11.1305, 10.5992,
          9.04656, 7.30333, 6.11291, 6.59994, 9.8862, 14.2276, 19.2682,
          22.5998, 26.3267, 33.37, 59.2035, 58.8323, 56.0236, 32.6866,
          21.8327, 19.1471, 19.9451, 23.6452, 32.5672, 41.4005, 56.5703,
          82.8037, 114.893, 112.824, 79.6278, 48.2594, 28.5979, 17.619,
          11.493, 5.87904, 3.8114, 2.71883, 2.68868, 3.11531, 3.59612,
          4.12546, 4.87729, 5.48662, 6.5506, 7.53293, 9.40118, 11.1709,
          14.3614, 21.1263, 27.5253, 37.3192, 51.8651, 28.7732, 17.0467,
          11.5986, 9.04432, 7.7853, 6.84385, 6.71141, 7.2079, 8.12153,
          10.8557, 15.0913, 24.6041, 24.191 
        };

        auto elastic = reconstructed.at( eID ).front();
        auto capture = reconstructed.at( fID ).front();
        // auto fission = reconstructed.at( cID ).front();
        details::checkRanges( refEnergies, elastic.x( ) );
        details::checkRanges( refElastic, elastic.y() );
        details::checkRanges( refEnergies, capture.x( ) );
        details::checkRanges( refCapture, capture.y() );
        // details::checkRanges( refEnergies, fission.x( ) );
        // details::checkRanges( refFission, fission.y() );
        
      } // THEN
      THEN( "the unresolved resonance keys can also be verified" ){
        const auto& unresolved = r2d2.unresolved();

        std::vector< ReactionID > refKeys{ eID, cID };
        auto keys = ranges::view::keys( unresolved ) | ranges::to_vector;

        std::sort( refKeys.begin(), refKeys.end() );
        std::sort( keys.begin(), keys.end() );
        CHECK( ranges::equal( refKeys, keys ) );

      } // THEN
      THEN( "the unresolved capture cross section can be verified" ){
        const auto& unresolved = r2d2.unresolved();

        std::vector< double > refEnergies{
          15000,  17000,  20000,   25000,  30000,  
          35000,  40000,  50000,   60000,  70000,  
          80000,  90000,  100000
        };
        std::vector< double > refXS{
          0.00500132,   0.00442008,  0.00376465,   0.00301958,   
          0.00252133,   0.00216459,  0.0018965,    0.00152033,   
          0.00126893,   0.001089,    0.000953847,  0.000848589,  
          0.000764289
        };
        auto capture = unresolved.at( cID );
        auto crossSections = capture.crossSections< 
            njoy::RECONR::interp::LinearLinear >();

        details::checkRanges( refEnergies, crossSections.x() );
        details::checkRanges( refXS, crossSections.y() );
      } // THEN
      THEN( "the unresolved elastic cross section can be verified" ){
        const auto& unresolved = r2d2.unresolved();

        std::vector< double > refEnergies{
          15000,  17000,  20000,   25000,  30000,  
          35000,  40000,  50000,   60000,  70000,  
          80000,  90000,  100000
        };
        std::vector< double > refXS{
          8.73463,  8.4211,   8.03861,  7.55437,  
          7.18993,  6.90114,  6.6638,   6.29054,  
          6.00425,  5.77319,  5.57992,  5.41392,  
          5.26844
        };
        auto elastic = unresolved.at( eID );
        auto crossSections = elastic.crossSections< 
            njoy::RECONR::interp::LinearLinear >();

        details::checkRanges( refEnergies, crossSections.x() );
        details::checkRanges( refXS, crossSections.y() );
      } // THEN
      
    } // WHEN
    
  } // GIVEN
  GIVEN( "an RML R2D2 object and reference grid" ){
    auto material = details::ENDFMaterial( "RML" );
    auto r2d2 = njoy::RECONR::R2D2::Factory()( logger, material );

    auto proj = r2d2.projectile();
    auto target = r2d2.target();

    auto eID = ReactionID{ proj, target, ReactionType{ 2 } };
    auto fID = ReactionID{ proj, target, ReactionType{ 19 } };
    auto cID = ReactionID{ proj, target, ReactionType{ 102 } };

    std::vector< double > grid{ 1e-05, 77.88, 515.2, 535.9, 5500 };
    
    WHEN( "the resonances are reconstructed" ){
      tRECONR::reconstructResonances( logger, grid, r2d2, 1E-1, 1E-5 );

      auto reconstructed = r2d2.reconstructedResonances();

      std::vector< njoy::elementary::ReactionID > refIDs{ eID, cID };
      auto IDs = ranges::view::keys( reconstructed ) | ranges::to_vector;
      std::sort( refIDs.begin(), refIDs.end() );
      std::sort( IDs.begin(), IDs.end() );

      REQUIRE( ranges::equal( refIDs, IDs ) );

      std::vector< double > refE{ 
        1e-05,        1.9284e-05,   2.8568e-05,   4.71361e-05,  8.42721e-05,  
        0.000158544,  0.000307089,  0.000604177,  0.00119835,   0.00238671,   
        0.00476342,   0.00951683,   0.0190237,    0.0380373,    0.0760647,    
        0.152119,     0.304229,     0.608447,     1.21688,      2.43376,      
        4.86751,      9.73501,      19.47,        38.94,        77.88,        
        187.21,       296.54,       515.2,        535.9,        1156.41,      
        1776.92,      2397.44,      2707.69,      3017.95,      3173.08,      
        3250.64,      3289.42,      3328.21,      3347.6,       3357.29,      
        3366.99,      3376.68,      3386.38,      3396.07,      3405.77,      
        3415.47,      3425.16,      3444.55,      3483.33,      3560.9,       
        3638.46,      3793.59,      3948.72,      4258.98,      4569.23,      
        4879.49,      5189.74,      5500
      };
      std::map< njoy::RECONR::ReactionID, std::vector< double > > refXS{
        { cID, {
          69.0744,     49.7415,     40.8674,     31.8156,    23.7944,     
          17.3477,     12.4648,     8.88659,     6.30993,    4.47114,     
          3.16489,     2.23909,     1.5837,      1.12,       0.792016,    
          0.56007,     0.396051,    0.280074,    0.198074,   0.140104,    
          0.09913,     0.0701831,   0.049751,    0.0353558,  0.0252528,   
          0.0167582,   0.0137056,   0.0110307,   0.0108772,  0.00884997,  
          0.00868279,  0.00928503,  0.00982943,  0.0105514,  0.0109877,   
          0.0112268,   0.011352,    0.0114809,   0.0115469,  0.0115802,   
          0.0116138,   0.0116477,   0.0116818,   0.0117162,  0.0117508,   
          0.0117857,   0.0118208,   0.011892,    0.0120374,  0.0123418,   
          0.0126651,   0.0133734,   0.014174,    0.0161113,  0.0186317,   
          0.0219723,   0.0265079,   0.0328557 }
        },
        { eID, {
          1.19112,      1.19112,      1.19112,      1.19112,      1.19112,      
          1.19112,      1.19112,      1.19112,      1.19111,      1.19111,      
          1.19111,      1.19111,      1.19111,      1.1911,       1.19108,      
          1.19105,      1.19099,      1.19087,      1.19063,      1.19014,      
          1.18917,      1.18723,      1.18334,      1.17555,      1.15993,      
          1.11581,      1.0713,       0.981226,     0.97263,      0.711254,     
          0.450493,     0.211276,     0.111987,     0.0374512,    0.0134382,    
          0.00564987,   0.00294592,   0.00109261,   0.000499135,  0.000288264,  
          0.000135642,  4.20358e-05,  8.22216e-06,  3.49872e-05,  0.000123128,  
          0.000273451,  0.000486775,  0.00110575,   0.0031369,    0.0105729,    
          0.0229191,    0.0645751,    0.133241,     0.378896,     0.834026,     
          1.61942,      2.93878,      5.15011 }
        }
      };

      for( const auto& id : IDs ){
        THEN( "the reconstructed cross section can be verified" +
              " for reaction: " + id.symbol() ){
          auto reaction = reconstructed.at( id ).front();

          details::checkRanges( refE, reaction.x() );
          details::checkRanges( refXS.at( id ), reaction.y() );
          
        } // THEN
      }
      THEN( "the unresolved resonance keys can also be verified" ){
        const auto& unresolved = r2d2.unresolved();

        std::vector< ReactionID > refKeys{ eID, cID };
        auto keys = ranges::view::keys( unresolved ) | ranges::to_vector;

        std::sort( refKeys.begin(), refKeys.end() );
        std::sort( keys.begin(), keys.end() );
        CHECK( ranges::equal( refKeys, keys ) );

      } // THEN
      THEN( "the unresolved capture cross section can be verified" ){
        const auto& unresolved = r2d2.unresolved();

        std::vector< double > refEnergies{
          15000,  17000,  20000,   25000,  30000,  
          35000,  40000,  50000,   60000,  70000,  
          80000,  90000,  100000
        };
        std::vector< double > refXS{
          0.00500132,   0.00442008,  0.00376465,   0.00301958,   
          0.00252133,   0.00216459,  0.0018965,    0.00152033,   
          0.00126893,   0.001089,    0.000953847,  0.000848589,  
          0.000764289
        };
        auto capture = unresolved.at( cID );
        auto crossSections = capture.crossSections< 
            njoy::RECONR::interp::LinearLinear >();

        details::checkRanges( refEnergies, crossSections.x() );
        details::checkRanges( refXS, crossSections.y() );
      } // THEN
      THEN( "the unresolved elastic cross section can be verified" ){
        const auto& unresolved = r2d2.unresolved();

        std::vector< double > refEnergies{
          15000,  17000,  20000,   25000,  30000,  
          35000,  40000,  50000,   60000,  70000,  
          80000,  90000,  100000
        };
        std::vector< double > refXS{
          8.73463,  8.4211,   8.03861,  7.55437,  
          7.18993,  6.90114,  6.6638,   6.29054,  
          6.00425,  5.77319,  5.57992,  5.41392,  
          5.26844
        };
        auto elastic = unresolved.at( eID );
        auto crossSections = elastic.crossSections< 
            njoy::RECONR::interp::LinearLinear >();

        details::checkRanges( refEnergies, crossSections.x() );
        details::checkRanges( refXS, crossSections.y() );
      } // THEN
    } // WHEN
  } // GIVEN
  
} // SCENARIO
