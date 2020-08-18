#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "RECONR.hpp"

#include "RECONR/details/simpleENDFTestString.hpp"
#include "RECONR/details/checkRanges.hpp"
#include "RECONR/details/printV.hpp"

SCENARIO( "Testing R2D2" ){
  GIVEN( "An ENDF file with SLBW resonance parameters" ){
    auto material = details::ENDFMaterial( "SLBW" );
    auto r2d2 = njoy::RECONR::R2D2::Factory()( material );

    THEN( "the pieces can be verified" ){
      
      const std::vector< double > resonanceEnergies { 
        0.00001, 0.9860692, 1.0253, 1.0645308, 2.9860692, 3.0253, 3.0645308, 7.5,
        std::nextafter( 7.5, std::numeric_limits< double >::infinity() ),
        1.5E4, 17906.6, 20000, 23875.5, 28502.1, 30000, 35813.3, 40000, 47751.1, 
        50000, 59688.8, 60000, 70000, 80000, 90000, 100000, 100000, 100000
      };
      details::checkRanges( resonanceEnergies, r2d2.resonanceReferenceGrid( ) );
    } // THEN
    
  } // GIVEN
  GIVEN( "An ENDF file with RM resonance parameters" ){
    auto material = details::ENDFMaterial( "RM" );
    auto r2d2 = njoy::RECONR::R2D2::Factory()( material );

    THEN( "the pieces can be verified" ){
      
      const std::vector< double > resonanceEnergies { 
        0.98596,  1.06389,  1.1389,   1.21391,  2.02109,  
        2.0361,   2.05111,  2.74827,  2.7767,   2.80513,  
        3.14517,  3.1566,   3.16803,  3.60111,  3.6208,   
        3.64049,  4.8336,   4.8508,   4.868,    5.24932,  
        5.4497,   5.5,      5.5,      15000,    17906.6,  
        20000,    23875.5,  28502.1,  30000,    35813.3,  
        40000,    47751.1,  50000,    59688.8,  60000,    
        70000,    80000,    90000,    100000,   100000,   
        100000
      };
      details::checkRanges( resonanceEnergies, r2d2.resonanceReferenceGrid( ) );
    } // THEN
    
  } // GIVEN
  GIVEN( "An ENDF file with RML resonance parameters" ){
    auto material = details::ENDFMaterial( "RML" );
    auto r2d2 = njoy::RECONR::R2D2::Factory()( material );

    THEN( "the pieces can be verified" ){
      
      const std::vector< double > resonanceEnergies { 
        1e-05,   5500,     5500,     15000,    17906.6,  
        20000,   23875.5,  28502.1,  30000,    35813.3,  
        40000,   47751.1,  50000,    59688.8,  60000,    
        70000,   80000,    90000,    100000,   100000,   
        100000
      };
      auto trial = r2d2.resonanceReferenceGrid();
      details::checkRanges( resonanceEnergies,  trial );
    } // THEN
    
  } // GIVEN
  
} // SCENARIO
