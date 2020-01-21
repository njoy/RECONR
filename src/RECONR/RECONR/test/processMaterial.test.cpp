
#include "catch.hpp"

#include "RECONR.hpp"

SCENARIO( "Processing a material" ){
  GIVEN( "an ENDFtk::syntaxTree::Material" ){
    auto tape = njoy::ENDFtk::syntaxTree::makeTape( 
      njoy::utility::slurpFileToMemory( "tape20" ) );
    nlohmann::json sequence = R"({
      "mat": 2631, "ncards": 2, "ngrid": 0,
      "err": 0.067, "tempr": 3.0, "errmax": 2.1, "errint": 8E-7,
      "cards": [ "Material 2631 processed with modern RECONR",
                  "For testing purposes only." ],
      "enode": [ ]
    })"_json;
    auto material = tape.MAT( sequence[ "mat" ] );

    THEN( "the material can be processed" ){
      njoy::RECONR::RECONR::processMaterial( sequence, material );
    } // THEN
  } // GIVEN
  
} // SCENARIO
