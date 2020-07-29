SCENARIO( "Getting evaluated data" ){
  WHEN( "Getting an existant ENDF Tape" ){
    auto evaluatedData = tRECONR::getEvaluated(
      std::cout, "tape20" );

    auto tape = std::get< 0 >( evaluatedData );

    GIVEN( "MAT numbers in the tape" ){
      const std::vector< int > validMATs{ 125, 2631, 9228 };
      for( auto MAT : validMATs ){
        THEN( "we can get Material: " + std::to_string( MAT ) ){
          CHECK( tape.hasMAT( MAT ) );
          CHECK_NOTHROW( tape.MAT( MAT ) );
        } // THEN
      }
    } // GIVEN
  } // WHEN
  WHEN( "Getting an non-existant ENDF Tape" ){
    THEN( "an exception is thrown" ){
      CHECK_THROWS( tRECONR::getEvaluated( std::cout, "tape45" ) );
    } // THEN

  } // WHEN
} // SCENARIO
