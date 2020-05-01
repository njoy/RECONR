#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "RECONR.hpp"

#include "RECONR/details/simpleENDFTestString.hpp"

SCENARIO( "Testing R2D2" ){
  GIVEN( "An ENDF file with SLBW resonance parameters" ){
    auto material = details::ENDFMaterial( "SLBW" );
    auto r2d2 = njoy::RECONR::R2D2::Factory( std::move( material ) )();

    THEN( "the pieces can be verified" ){
      
      const std::vector< double > resonanceEnergies { 
       -1.000000E+2, -9.000000E+1, -4.297600E+0, -3.493400E+0,
       -1.504300E+0, -4.116100E-1, -1.942800E-1,  3.657500E-5,
        2.819000E-1,  1.138900E+0,  2.036100E+0,  2.776700E+0,
        3.156600E+0,  3.620800E+0,  4.850800E+0,  5.449700E+0 };

      CHECK( resonanceEnergies == r2d2.resonanceReferenceGrid() );
    } // THEN
    
  } // GIVEN
  
} // SCENARIO
