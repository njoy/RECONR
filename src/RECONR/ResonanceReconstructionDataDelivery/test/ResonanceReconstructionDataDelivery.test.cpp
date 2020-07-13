#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "RECONR.hpp"

#include "RECONR/details/simpleENDFTestString.hpp"
#include "RECONR/details/checkRanges.hpp"

SCENARIO( "Testing R2D2" ){
  GIVEN( "An ENDF file with SLBW resonance parameters" ){
    auto material = details::ENDFMaterial( "SLBW" );
    auto r2d2 = njoy::RECONR::R2D2::Factory( std::move( material ) )();

    THEN( "the pieces can be verified" ){
      
      const std::vector< double > resonanceEnergies { 
        0.00001, 0.9860692, 1.0253, 1.0645308, 2.9860692, 3.0253, 3.0645308, 7.5,
        std::nextafter( 7.5, std::numeric_limits< double >::infinity() )
      };
      details::checkRanges( resonanceEnergies, r2d2.resonanceReferenceGrid( ) );
    } // THEN
    
  } // GIVEN
  GIVEN( "An ENDF file with RM resonance parameters" ){
    auto material = details::ENDFMaterial( "RM" );
    auto r2d2 = njoy::RECONR::R2D2::Factory( std::move( material ) )();

    THEN( "the pieces can be verified" ){
      
      const std::vector< double > resonanceEnergies { 
        0.98596, 1.06389, 1.1389, 1.21391, 2.02109, 2.0361, 2.05111, 
        2.74827, 2.7767, 2.80513, 3.14517, 3.1566, 3.16803, 3.60111, 
        3.6208, 3.64049, 4.8336, 4.8508, 4.868, 5.24932, 5.4497, 5.5,
        std::nextafter( 5.5, std::numeric_limits< double >::infinity() )
      };
      details::checkRanges( resonanceEnergies, r2d2.resonanceReferenceGrid( ) );
    } // THEN
    
  } // GIVEN
  GIVEN( "An ENDF file with RML resonance parameters" ){
    auto material = details::ENDFMaterial( "RML" );
    auto r2d2 = njoy::RECONR::R2D2::Factory( std::move( material ) )();

    THEN( "the pieces can be verified" ){
      
      const std::vector< double > resonanceEnergies { 
        1.0E-5, 7.788000e+3, 5.152000e+4, 5.359000e+4, 5.5E5,
        std::nextafter( 5.5E5, std::numeric_limits< double >::infinity() )
      };
      auto trial = r2d2.resonanceReferenceGrid();
      details::checkRanges( resonanceEnergies,  trial );
    } // THEN
    
  } // GIVEN
  
} // SCENARIO
