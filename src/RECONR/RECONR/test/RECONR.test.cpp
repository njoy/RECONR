#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "RECONR.hpp"

nlohmann::json input = R"({
  "nendf": 20, "npend": 22,
  "tlabel": "Modern RECONR Testing",
  "sequence": [
    {
      "mat": 125, "ncards": 1, "ngrid": 3,
      "err": 0.005, "tempr": 0, "errmax": 0.1, "errint": 5E-7,
      "cards": [ "Material 125 processed with modern RECONR" ],
      "enode": [ 1.0, 2.0, 3.0 ]
    },
    {
      "mat": 2631, "ncards": 2, "ngrid": 0,
      "err": 0.067, "tempr": 3.0, "errmax": 2.1, "errint": 8E-7,
      "cards": [ "Material 2631 processed with modern RECONR",
                  "For testing purposes only." ],
      "enode": [ ]
    },
    {
      "mat": 9228, "ncards": 1, "ngrid": 0,
      "err": 0.067, "tempr": 3.0, "errmax": 2.1, "errint": 8E-7,
      "cards": [ "Material 9228 processed with modern RECONR" ],
      "enode": [ ]
    }
  ]
})"_json;

SCENARIO( "Testing creation of RECONR class" ){
  GIVEN( "a JSON object, and extra arguments" ){

    auto args = nlohmann::json::object();

    WHEN( "a RECONR object is called" ){
      // CHECK_NOTHROW( njoy::RECONR::RECONR()( input, args ) );

    } // THEN
  } // GIVEN
  
} // SCENARIO
