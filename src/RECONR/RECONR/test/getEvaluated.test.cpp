#include "catch.hpp"

#include "RECONR.hpp"

extern nlohmann::json input;
SCENARIO( "Getting evaluated data" ){
  WHEN( "Getting an ENDF Tape" ){
    auto evaluatedData = njoy::RECONR::RECONR::getEvaluated( input[ "nendf" ] );

    auto tape = mpark::get< 0 >( evaluatedData );

    GIVEN( "MAT numbers in the tape" ){
      const std::vector< int > validMATs{ 125, 2631, 9228 };
      for( auto MAT : validMATs ){
        THEN( "we can get Material: " + std::to_string( MAT ) ){
          CHECK( tape.hasMAT( MAT ) );
          CHECK_NOTHROW( tape.MAT( MAT ) );
        } // THEN
      }

      /* This works, but takes far too long to run to be useful
       *
      auto invalidMATs = ranges::view::iota( 1, 999 )
        | ranges::view::remove_if( 
          [&](int MAT ){ return ranges::binary_search( validMATs, MAT ); } );

      for( auto MAT : invalidMATs ){
        THEN( "we get exceptions for MAT: " + std::to_string( MAT ) ){
          CHECK( not tape.hasMAT( MAT ) );
          CHECK_THROWS( tape.MAT( MAT ) );
        } // THEN
      }
      */
    } // GIVEN
  } // WHEN
} // SCENARIO
