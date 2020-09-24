SCENARIO( "Testing the linearization of collected cross sections" ){
  GIVEN( "an ResonanceReconstructionDataDelivery object" ){
    auto material = details::ENDFMaterial( "SLBW" );
    auto r2d2 = njoy::RECONR::R2D2::Factory()( logger, material );

    auto projectile = r2d2.projectile();
    auto target = r2d2.target();

    WHEN( "the cross sections are linearized" ){
      double absTolerance{ 1E-6 };
      double relTolerance{ 1E-1 }; // This tolerance is large by design
      tRECONR::linearizeXS( logger, r2d2, absTolerance, relTolerance );

      auto reactions = r2d2.reactions();

      using PID = ParticleID;
      using NID = NucleusID;

      std::vector< njoy::RECONR::ReactionID > refKeys{ 
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
      auto keys = ranges::view::keys( reactions ) | ranges::to_vector;
      std::sort( refKeys.begin(), refKeys.end() );
      std::sort( keys.begin(), keys.end() );
      CHECK( ranges::equal( refKeys, keys ) );

      auto skeys = keys 
        | ranges::view::transform( []( auto&& k ){ return k.symbol(); } )
        | ranges::to_vector;
      auto sref = refKeys 
        | ranges::view::transform( []( auto&& k ){ return k.symbol(); } )
        | ranges::to_vector;
      
      using Rxn_t = njoy::RECONR::interp::LinearTable;
      // njoy::RECONR::ReactionID MT;
      THEN( "we can check MT=18" ){
        ReactionID reactionID{ projectile, target, ReactionType{ 18 } };
        auto reaction = reactions.at( reactionID );
        std::vector< double > refE{ 
          100000,  125000,   137500,    143750,  146875,  
          148438,  149219,   149609,    149805,  149902,  
          149951,  149976,   149988,    149994,  149997,  
          149998,  149999,   150000,    150000,  150000,  
          150000,  150000,   150000,    150000,  150000,  
          150000,  150000,   150000,    150000,  150000,  
          150000,  150000,   150000,    150000,  150000,  
          150000,  150000,   150000,    150000,  150000,  
          150000,  150000,   150000,    150000,  150000,  
          150000,  150000,   150000,    150000,  150000,  
          150000,  150000,   150000,    450000,  600000,  
          675000,  712500,   731250,    740625,  745312,  
          747656,  748828,   749414,    749707,  749854,  
          749927,  749963,   749982,    749991,  749995,  
          749998,  749999,   749999,    750000,  750000,  
          750000,  750000,   750000,    750000,  750000,  
          750000,  750000,   750000,    750000,  750000,  
          750000,  750000,   750000,    750000,  750000,  
          750000,  750000,   750000,    750000,  750000,  
          750000,  750000,   750000,    750000,  750000,  
          750000,  750000,   750000,    750000,  750000,  
          750000,  1.9e+07,  1.95e+07,  2e+07
        };
        std::vector< double > refB{ 
          18,           18,           18,          18,           18,      
          18,           18,           18,          18,           18,      
          18,           18,           18,          18,           18,      
          18,           18,           18,          18,           18,      
          18,           18,           18,          18,           18,      
          18,           18,           18,          18,           18,      
          18,           18,           18,          18,           18,      
          18,           18,           18,          18,           18,      
          18,           18,           18,          18,           18,      
          18,           18,           18,          18,           18,      
          18,           18,           11.829,      11.829,       11.829,  
          11.829,       11.829,       11.829,      11.829,       11.829,  
          11.829,       11.829,       11.829,      11.829,       11.829,  
          11.829,       11.829,       11.829,      11.829,       11.829,  
          11.829,       11.829,       11.829,      11.829,       11.829,  
          11.829,       11.829,       11.829,      11.829,       11.829,  
          11.829,       11.829,       11.829,      11.829,       11.829,  
          11.829,       11.829,       11.829,      11.829,       11.829,  
          11.829,       11.829,       11.829,      11.829,       11.829,  
          11.829,       11.829,       11.829,      11.829,       11.829,  
          11.829,       11.829,       11.829,      11.829,       11.829,  
          3.34739e-05,  2.75176e-05,  2.7313e-05,  2.71079e-05
        };

        auto lXS = reaction.crossSections< Rxn_t >();
        auto energies = lXS.x() | ranges::to_vector;
        auto barns = lXS.y() | ranges::to_vector;
        details::checkRanges( refE, energies );
        details::checkRanges( refB, barns );
      }
      THEN( "we can check MT=102" ){
        ReactionID reactionID{ projectile, target, ReactionType{ 102 } };
        auto reaction = reactions.at( reactionID );
				std::vector< double > refE{
					1e-05,        1.0625e-05,   1.125e-05,    1.1875e-05,   1.25e-05,     
					1.3125e-05,   1.375e-05,    1.4375e-05,   1.5e-05,      1.5625e-05,   
					1.625e-05,    1.75e-05,     1.875e-05,    2e-05,
					6.36557e-05,  0.000107311,  0.000194623,  0.000369246,  0.000718492,  
					0.00141698,   0.00281397,   0.00560794,   0.0111959,    0.0223717,    
					0.0447235,    0.089427,     0.178834,     0.357648,     0.715276,     
					1.43053,      2.86104,      5.72207,      11.4441,      22.8882,      
					45.7764,      91.5528,      183.105,      366.211,      732.422,      
					1464.84,      2929.69,      5859.38,      11718.8,      23437.5,      
					46875,        93750,        187500,       375000,       750000,       
					1.9e+07,      1.95e+07,     2e+07 };
				std::vector< double > refB {
					102,          94.1134,      86.6777,     79.6441,     72.9714,         
					66.6243,      60.5726,      54.7899,     49.2533,     43.9428,         
					38.8406,      29.2,         20.2247,     11.829,
					11.2665,      11.0128,      10.7235,     10.4124,     10.089,          
					9.75904,      9.42573,      9.0907,      8.75481,     8.41849,         
					8.08195,      7.7453,       7.4086,      7.07187,     6.73513,         
					6.39838,      6.06163,      5.72488,     5.38812,     5.05137,         
					4.71461,      4.37786,      4.0411,      3.70435,     3.36759,         
					3.03083,      2.69408,      2.35732,     2.02057,     1.68381,         
					1.34706,      1.0103,       0.673545,    0.336789,    3.34739e-05,     
					2.75176e-05,  2.7313e-05,  2.71079e-05 };

        auto lXS = reaction.crossSections< Rxn_t >();
        auto energies = lXS.x() | ranges::to_vector;
        auto barns = lXS.y() | ranges::to_vector;
        CHECK( ranges::distance( refE ) == ranges::distance( energies ) );
        CHECK( ranges::distance( refB ) == ranges::distance( barns ) );
        for( const auto& [r, e ] : ranges::view::zip( refE, energies ) ){
          CHECK( r == Approx( e ).epsilon( 5E-6 ) );
        }
        for( const auto& [r, b ] : ranges::view::zip( refB, barns ) ){
          CHECK( r == Approx( b ).epsilon( 5E-6 ) );
        }
      }
    } // THEN
  } // GIVEN
} // SCENARIO
