SCENARIO( "Testing the unionization of the energy Grid" ){
  // These are the same regardless of the resonance formalism
  std::vector< double > userSupplied{ 1.0, 2.0, 3.0 };
  auto energies = XSEnergies();
  WHEN( "resonances have not been reconstructed" ){
    
    GIVEN( "a linearized ResonanceReconstructionDataDelivery (SLBW) object" ){
      auto material = details::ENDFMaterial( "SLBW" );
      auto r2d2 = njoy::RECONR::R2D2::Factory()( logger, material );
    
      double absTolerance{ 1E-6 };
      double relTolerance{ 1E-1 }; // This tolerance is large by design
      tRECONR::linearizeXS( logger, r2d2, absTolerance, relTolerance );
    
      THEN( "the energygrid can be unionized" ){
        std::vector< double > refGrid{
          1e-05,        1.0625e-05,   1.125e-05,    1.1875e-05,   1.25e-05,     
          1.3125e-05,   1.375e-05,    1.4375e-05,   1.5e-05,      1.5625e-05,   
          1.625e-05,    1.75e-05,     1.875e-05,    2e-05,        2e-05,        
          6.36557e-05,  0.000107311,  0.000194623,  0.000369246,  0.000718492,  
          0.00141698,   0.00281397,   0.00560794,   0.0111959,    0.0223717,    
          0.0447235,    0.089427,     0.1,          0.178834,     0.357648,     
          0.715276,     0.986069,     1,            1.0253,       1.06453,      
          1.43053,      2,            2.86104,      2.98607,      3,            
          3.0253,       3.06453,      5.72207,      7.5,          7.5,          
          11.4441,      22.8882,      45.7764,      91.5528,      183.105,      
          322.465,      366.211,      732.422,      1464.84,      2929.69,      
          5859.38,      11718.8,      15000,        17906.6,      20000,        
          23437.5,      23875.5,      28502.1,      30000,        35813.3,      
          40000,        46875,        47751.1,      50000,        59688.8,      
          60000,        70000,        80000,        90000,        93750,        
          100000,       100000,       150000,       150000,       187500,       
          375000,       750000,       750000,       1.09e+06,     2e+06,        
          3.22465e+06,  5e+06,        5.5e+06,      6e+06,        6.5e+06,      
          7e+06,        7.5e+06,      8e+06,        1.42e+07,     1.9e+07,      
          1.9e+07,      1.95e+07,     1.95e+07,     2e+07,        2e+07,        
          3e+07
        };
    
        auto trial = tRECONR::unionizeEnergyGrid( 
          logger, 
          r2d2.reactions(), 
          r2d2.photonProductions(), 
          r2d2.resonanceReferenceGrid(),
          userSupplied );
    
        details::checkRanges( refGrid, trial );
      } // THEN
    } // GIVEN
    GIVEN( "a linearized ResonanceReconstructionDataDelivery (RM) object" ){
      auto material = details::ENDFMaterial( "RM" );
      auto r2d2 = njoy::RECONR::R2D2::Factory()( logger, material );
    
      double absTolerance{ 1E-6 };
      double relTolerance{ 1E-1 }; // This tolerance is large by design
      tRECONR::linearizeXS( logger, r2d2, absTolerance, relTolerance );
    
      THEN( "the energygrid can be unionized" ){
        std::vector< double > refGrid { 
          1e-05,        1.0625e-05,   1.125e-05,    1.1875e-05,   1.25e-05,     
          1.3125e-05,   1.375e-05,    1.4375e-05,   1.5e-05,      1.5625e-05,   
          1.625e-05,    1.75e-05,     1.875e-05,    2e-05,        2e-05,        
          6.36557e-05,  0.000107311,  0.000194623,  0.000369246,  0.000718492,  
          0.00141698,   0.00281397,   0.00560794,   0.0111959,    0.0223717,    
          0.0447235,    0.089427,     0.1,          0.178834,     0.357648,     
          0.715276,     0.98596,      1,            1.06389,      1.1389,       
          1.21391,      1.43053,      2,            2.02109,      2.0361,       
          2.05111,      2.74827,      2.7767,       2.80513,      2.86104,      
          3,            3.14517,      3.1566,       3.16803,      3.60111,      
          3.6208,       3.64049,      4.8336,       4.8508,       4.868,        
          5.24932,      5.4497,       5.5,          5.5,          5.72207,      
          11.4441,      22.8882,      45.7764,      91.5528,      183.105,      
          322.465,      366.211,      732.422,      1464.84,      2929.69,      
          5859.38,      11718.8,      15000,        17906.6,      20000,        
          23437.5,      23875.5,      28502.1,      30000,        35813.3,      
          40000,        46875,        47751.1,      50000,        59688.8,      
          60000,        70000,        80000,        90000,        93750,        
          100000,       100000,       150000,       150000,       187500,       
          375000,       750000,       750000,       1.09e+06,     2e+06,        
          3.22465e+06,  5e+06,        5.5e+06,      6e+06,        6.5e+06,      
          7e+06,        7.5e+06,      8e+06,        1.42e+07,     1.9e+07,      
          1.9e+07,      1.95e+07,     1.95e+07,     2e+07,        2e+07,        
          3e+07
        };
    
        auto trial = tRECONR::unionizeEnergyGrid( 
          logger, 
          r2d2.reactions(), 
          r2d2.photonProductions(), 
          r2d2.resonanceReferenceGrid(),
          userSupplied );
    
        details::checkRanges( refGrid, trial );
      } // THEN
    } // GIVEN
    GIVEN( "a linearized ResonanceReconstructionDataDelivery (RML) object" ){
      auto material = details::ENDFMaterial( "RML" );
      auto r2d2 = njoy::RECONR::R2D2::Factory()( logger, material );
    
      double absTolerance{ 1E-6 };
      double relTolerance{ 1E-1 }; // This tolerance is large by design
      tRECONR::linearizeXS( logger, r2d2, absTolerance, relTolerance );
    
      THEN( "the energygrid can be unionized" ){
        std::vector< double > refGrid { 
          1e-05,        1.0625e-05,   1.125e-05,    1.1875e-05,   1.25e-05,     
          1.3125e-05,   1.375e-05,    1.4375e-05,   1.5e-05,      1.5625e-05,   
          1.625e-05,    1.75e-05,     1.875e-05,    2e-05,        2e-05,        
          6.36557e-05,  0.000107311,  0.000194623,  0.000369246,  0.000718492,  
          0.00141698,   0.00281397,   0.00560794,   0.0111959,    0.0223717,    
          0.0447235,    0.089427,     0.1,          0.178834,     0.357648,     
          0.715276,     1,            1.43053,      2,            2.86104,      
          3,            5.72207,      11.4441,      22.8882,      45.7764,      
          91.5528,      183.105,      322.465,      366.211,      732.422,      
          1464.84,      2929.69,      5500,         5500,         5859.38,      
          11718.8,      15000,        17906.6,      20000,        23437.5,      
          23875.5,      28502.1,      30000,        35813.3,      40000,        
          46875,        47751.1,      50000,        59688.8,      60000,        
          70000,        80000,        90000,        93750,        100000,       
          100000,       150000,       150000,       187500,       375000,       
          750000,       750000,       1.09e+06,     2e+06,        3.22465e+06,  
          5e+06,        5.5e+06,      6e+06,        6.5e+06,      7e+06,        
          7.5e+06,      8e+06,        1.42e+07,     1.9e+07,      1.9e+07,      
          1.95e+07,     1.95e+07,     2e+07,        2e+07,        3e+07
        };
    
        auto trial = tRECONR::unionizeEnergyGrid( 
          logger, 
          r2d2.reactions(), 
          r2d2.photonProductions(), 
          r2d2.resonanceReferenceGrid(),
          userSupplied );
    
        details::checkRanges( refGrid, trial );
      } // THEN
    } // GIVEN
  } // WHEN

  WHEN( "resonances have been reconstructed" ){
    GIVEN( "a linearized ResonanceReconstructionDataDelivery (SLBW) object" ){
      auto material = details::ENDFMaterial( "SLBW" );
      auto r2d2 = njoy::RECONR::R2D2::Factory()( logger, material );
    
      double absTolerance{ 1E-6 };
      double relTolerance{ 1E-1 }; // This tolerance is large by design
      tRECONR::linearizeXS( logger, r2d2, absTolerance, relTolerance );
      auto refGrid = tRECONR::unionizeEnergyGrid( 
          logger, 
          r2d2.reactions(), 
          r2d2.photonProductions(), 
          r2d2.resonanceReferenceGrid(),
          userSupplied );
      auto energies = refGrid;
      tRECONR::reconstructResonances( logger, refGrid, r2d2, 1E-1, 1E-3 );
    
      THEN( "the reconstructed energygrid can be unionized" ){

        // refGrid is already verified above. Just need to add the
        // reconstructed/linearized cross sections
        for( const auto& [MT, V] : r2d2.reconstructedResonances() ){
          for( const auto& XS : V ){
            refGrid |= ranges::action::push_back( XS.x() );
          }
        }
        ranges::sort( refGrid );
        refGrid = ranges::view::unique( refGrid );

        auto trial = tRECONR::unionizeEnergyGrid( 
          logger, 
          energies,
          r2d2.reconstructedResonances() );

        details::checkRanges( refGrid, trial );
      } // THEN
    } // GIVEN
    GIVEN( "a linearized ResonanceReconstructionDataDelivery (RM) object" ){
      auto material = details::ENDFMaterial( "RM" );
      auto r2d2 = njoy::RECONR::R2D2::Factory()( logger, material );
    
      double absTolerance{ 1E-6 };
      double relTolerance{ 1E-1 }; // This tolerance is large by design
      tRECONR::linearizeXS( logger, r2d2, absTolerance, relTolerance );
      auto refGrid = tRECONR::unionizeEnergyGrid( 
          logger, 
          r2d2.reactions(), 
          r2d2.photonProductions(), 
          r2d2.resonanceReferenceGrid(),
          userSupplied );
      auto energies = refGrid;
      tRECONR::reconstructResonances( logger, refGrid, r2d2, 1E-1, 1E-3 );
    
      THEN( "the reconstructed energygrid can be unionized" ){

        // refGrid is already verified above. Just need to add the
        // reconstructed/linearized cross sections
        for( const auto& [MT, V] : r2d2.reconstructedResonances() ){
          for( const auto& XS : V ){
            refGrid |= ranges::action::push_back( XS.x() );
          }
        }
        ranges::sort( refGrid );
        refGrid = ranges::view::unique( refGrid );

        auto trial = tRECONR::unionizeEnergyGrid( 
          logger, 
          energies,
          r2d2.reconstructedResonances() );

        details::checkRanges( refGrid, trial );
      } // THEN
    } // GIVEN
  } // WHEN
} // SCENARIO
