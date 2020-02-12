#include "catch.hpp"

#include "RECONR.hpp"

njoy::RECONR::R2D2::ENDFMaterial_t ENDFMaterial( std::string );

SCENARIO( "Testing the linearization of collected cross sections" ){
  GIVEN( "an ResonanceReconstructionDataDelivery object" ){
    auto material = ENDFMaterial( "SLBW" );
    njoy::RECONR::R2D2 r2d2{ std::move( material ) };

    THEN( "the cross sections can be linearized" ){
      CHECK_NOTHROW( r2d2.linearize() );
    } // THEN
    
  } // GIVEN
} // SCENARIO
