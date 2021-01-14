SCENARIO( "Testing creation of RECONR class" ){
  nlohmann::json input{R"({
    "nendf": 20, "npend": 22,
    "tlabel": "Modern RECONR Testing",
    "sequence": [
      {
        "mat": 125, "ncards": 1, "ngrid": 3,
        "err": 0.001, "tempr": 0, "errmax": 0.1, "errint": 5E-7,
        "cards": [ "Material 125 processed with modern RECONR" ],
        "enode": [ 1.0, 2.0, 3.0 ]
      },
      {
        "mat": 2631, "ncards": 2, "ngrid": 0,
        "err": 0.001, "tempr": 0.0, "errmax": 2.1, "errint": 8E-7,
        "cards": [ "Material 2631 processed with modern RECONR",
                    "For testing purposes only." ],
        "enode": [ ]
      }
    ]
  })"_json};
  nlohmann::json binInput{R"({
    "nendf": -20, "npend": 22,
    "tlabel": "Modern RECONR Testing",
    "sequence": [
      {
        "mat": 125, "ncards": 1, "ngrid": 3,
        "err": 0.001, "tempr": 0, "errmax": 0.1, "errint": 5E-7,
        "cards": [ "Material 125 processed with modern RECONR" ],
        "enode": [ 1.0, 2.0, 3.0 ]
      },
      {
        "mat": 2631, "ncards": 2, "ngrid": 0,
        "err": 0.001, "tempr": 0.0, "errmax": 2.1, "errint": 8E-7,
        "cards": [ "Material 2631 processed with modern RECONR",
                    "For testing purposes only." ],
        "enode": [ ]
      }
    ]
  })"_json};
  nlohmann::json Fe56Input{R"({
    "nendf": 20, "npend": 23,
    "tlabel": "Modern RECONR Testing",
    "sequence": [
      {
        "mat": 2631, "ncards": 2, "ngrid": 0,
        "err": 0.001, "tempr": 0.0, "errmax": 2.1, "errint": 8E-7,
        "cards": [ "Material 2631 processed with modern RECONR",
                    "For testing purposes only." ],
        "enode": [ ]
      }
    ]
  })"_json};
  nlohmann::json inputWithU235{R"({
    "nendf": 20, "npend": 24,
    "tlabel": "Modern RECONR Testing",
    "sequence": [
      {
        "mat": 125, "ncards": 1, "ngrid": 3,
        "err": 0.1, "tempr": 0, "errmax": 0.1, "errint": 5E-7,
        "cards": [ "Material 125 processed with modern RECONR" ],
        "enode": [ 1.0, 2.0, 3.0 ]
      },
      {
        "mat": 2631, "ncards": 2, "ngrid": 0,
        "err": 0.1, "tempr": 0.0, "errmax": 2.1, "errint": 8E-7,
        "cards": [ "Material 2631 processed with modern RECONR",
                    "For testing purposes only." ],
        "enode": [ ]
      },
      {
        "mat": 9228, "ncards": 1, "ngrid": 0,
        "err": 0.1, "tempr": 0.0, "errmax": 2.1, "errint": 8E-7,
        "cards": [ "Material 9228 processed with modern RECONR" ],
        "enode": [ ]
      }
    ]
  })"_json};
  nlohmann::json fastU235{R"({
    "nendf": 20, "npend": 25,
    "tlabel": "Modern RECONR Testing",
    "sequence": [
      {
        "mat": 9228, "ncards": 2, "ngrid": 0,
        "err": 0.1, "tempr": 0.0, "errmax": 2.1, "errint": 8E-7,
        "cards": [ "Material 9228 processed with modern RECONR",
                    "For testing purposes only." ],
        "enode": [ ]
      }
    ]
  })"_json};

  GIVEN( "a JSON object, and extra arguments" ){

    auto args = nlohmann::json::object();

    WHEN( "a RECONR object is called" ){
      CHECK_NOTHROW( tRECONR()( input, std::cout, std::cerr, args ) );
    } // WHEN
    WHEN( "binary tapes are used as input" ){
      CHECK_THROWS( tRECONR()( binInput, std::cout, std::cerr, args ) );
    } // WHEN
    WHEN( "processing one ENDF Material in a Tape" ){
      CHECK_NOTHROW( tRECONR()( Fe56Input, std::cout, std::cerr, args ) );
    } // WHEN
    WHEN( "processing H-1, Fe-56, and U-235" ){
      CHECK_NOTHROW( tRECONR()( inputWithU235, std::cout, std::cerr, args ) );
    } // WHEN
    WHEN( "processing U-235 with large tolerances" ){
      CHECK_NOTHROW( tRECONR()( fastU235, std::cout, std::cerr, args ) );
    } // WHEN
  } // GIVEN

  WHEN( "calling the modern RECONR interface" ){
    nlohmann::json modern{ R"({
      "input": "tape20",
      "output": "Fe56.modern.pendf",
      "tolerance": 0.001,
      "energy grid": []
    })"_json };

    CHECK_THROWS( tRECONR()( std::cout, std::cerr, modern ) );
  } // WHEN

} // SCENARIO
