#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "RECONR.hpp"

SCENARIO( "Testing creation of RECONR class" ){
  GIVEN( "a JSON object, and extra arguments" ){
    nlohmann::json input = R"({
    "nendf": 21, "npend": 22,
    "tlabel": "This is a sample Card2",
    "sequence": [
      {
        "mat": 1306, "ncards": 1, "ngrid": 3,
        "err": 0.005, "tempr": 0, "errmax": 0.1, "errint": 5E-7,
        "cards": [ "This a sample description TEXT Record for Card5" ],
        "enode": [ 1.0, 2.0, 3.0 ]
      }
    ]
    })"_json;

    auto args = nlohmann::json::object();

    WHEN( "creating a RECONR object" ){
      njoy::RECONR::RECONR reconr{ njoy::utility::copy( input ), args };

      THEN( "the members can be verified" ){
        CHECK( input == reconr.njoyArgs );
      }
    } // THEN
  } // GIVEN
  
} // SCENARIO
