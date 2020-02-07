#include "catch.hpp"

#include "RECONR.hpp"

extern nlohmann::json input;

SCENARIO( "Getting ResonanceReconstructionDataDelivery" ){
  GIVEN( "an ENDF Tape" ){
    auto evaluatedData = njoy::RECONR::RECONR::getEvaluated( input[ "nendf" ] );
    auto tape = mpark::get< 0 >( evaluatedData );
    THEN( "the resonance reconstruction data can be extracted" ){

    } // THEN
  } // GIVEN

} // SCENARIO
