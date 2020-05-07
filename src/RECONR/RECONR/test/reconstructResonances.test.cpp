#include "catch.hpp"

#include "RECONR.hpp"

#include "RECONR/details/simpleENDFTestString.hpp"

SCENARIO( "Testing the resonance reconstruction" ){
  GIVEN( "an SLBW R2D2 object and reference grid" ){
    auto material = details::ENDFMaterial( "SLBW" );
    auto r2d2 = njoy::RECONR::R2D2::Factory( std::move( material ) )();
    const std::vector< double > refGrid{
      0.00001, 0.9860692, 1.0253, 1.0645308, 2.9860692, 3.0253, 3.0645308, 7.5
    };

    WHEN( "the resonances are reconstructed" ){
      njoy::RECONR::RECONR::reconstructResonances( refGrid, r2d2, 1E-1, 1E-3 );
      
    } // WHEN
    
  } // GIVEN
  
} // SCENARIO
