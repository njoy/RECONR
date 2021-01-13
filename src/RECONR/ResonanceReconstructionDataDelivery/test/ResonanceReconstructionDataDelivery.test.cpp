#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "RECONR.hpp"

#include "RECONR/details/simpleENDFTestString.hpp"
#include "RECONR/details/checkRanges.hpp"
#include "RECONR/details/printV.hpp"

using namespace njoy;

RECONR::Logger logger{ std::cout, std::cerr };

SCENARIO( "Testing R2D2" ){
  GIVEN( "An ENDF file with SLBW resonance parameters" ){
    auto material = details::ENDFMaterial( "SLBW" );
    auto r2d2 = njoy::RECONR::R2D2::Factory()( logger, material );

    THEN( "the pieces can be verified" ){
      
      const std::vector< double > resonanceEnergies { 
        1e-05,    0.986069,  1.0253,   1.06453,  2.98607,  
        3.0253,   3.06453,   7.5,      15000,    17906.6,  
        21376.5,  25518.8,   30463.8,  36366.9,  43414,    
        51826.6,  61869.4,   73858.2,  88170.2,  100000
      };
      auto trial = r2d2.resonanceReferenceGrid();
      details::checkRanges( resonanceEnergies, trial );
    } // THEN
    
  } // GIVEN
  GIVEN( "An ENDF file with RM resonance parameters" ){
    auto material = details::ENDFMaterial( "RM" );
    auto r2d2 = njoy::RECONR::R2D2::Factory()( logger, material );

    THEN( "the pieces can be verified" ){
      
      const std::vector< double > resonanceEnergies { 
        0.98596,  1.06389,  1.1389,   1.21391,  2.02109,  
        2.0361,   2.05111,  2.74827,  2.7767,   2.80513,  
        3.14517,  3.1566,   3.16803,  3.60111,  3.6208,   
        3.64049,  4.8336,   4.8508,   4.868,    5.24932,  
        5.4497,   5.5,      15000,    17906.6,  21376.5,  
        25518.8,  30463.8,  36366.9,  43414,    51826.6,  
        61869.4,  73858.2,  88170.2,  100000
      };
      auto trial = r2d2.resonanceReferenceGrid();
      details::checkRanges( resonanceEnergies, trial );
    } // THEN
    
  } // GIVEN
  GIVEN( "An ENDF file with RML resonance parameters" ){
    auto material = details::ENDFMaterial( "RML" );
    auto r2d2 = njoy::RECONR::R2D2::Factory()( logger, material );

    THEN( "the pieces can be verified" ){
      
      const std::vector< double > resonanceEnergies { 
        1e-05,    7193.6,   7788,     8382.4,   15000,    
        17906.6,  21376.5,  25518.8,  30463.8,  36366.9,  
        43414,    51826.6,  61869.4,  73858.2,  88170.2,  
        100000
      };
      auto trial = r2d2.resonanceReferenceGrid();
      details::checkRanges( resonanceEnergies,  trial );
    } // THEN
    
  } // GIVEN
  
} // SCENARIO
