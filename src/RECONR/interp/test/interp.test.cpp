#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "RECONR.hpp"

#include "RECONR/details/interpLambdas.hpp"

using namespace njoy::RECONR;
SCENARIO( "Testing interpolation 'LAWS'." ){
  GIVEN( "known (x,y) values" ){
    std::vector< double > x1{ 1.0, 5.0, 8.0 };
    std::vector< double > y1{ 2.0, 7.0, 5.5 };
    std::vector< double > x2{ 8.0, 10.0, 18.0 };
    std::vector< double > y2{ 2.2, 7.2, 5.7 };

    WHEN( "creating a Histogram interpolation table (LAW=1)" ){
      interp::Histogram histo{ njoy::utility::copy( x1 ),
                               njoy::utility::copy( y1 ) };

      THEN( "we can verify interpolated values" ){
        // Given data
        CHECK( 2.0 == histo( 1.0 ) );
        CHECK( 7.0 == histo( 5.0 ) );
        CHECK( 5.5 == histo( 8.0 ) );

        // Somewhere inbetween
        CHECK( 2.0 == histo( 1.5 ) );
        CHECK( 7.0 == histo( 5.5 ) );
      } // THEN
    } // WHEN
    WHEN( "creating a LinearLinear interpolation table (LAW=2)" ){
      interp::LinearLinear linlin{ njoy::utility::copy( x1 ),
                                   njoy::utility::copy( y1 ) };
      THEN( "we can verify interpolated values" ){
        // Given data
        CHECK( 2.0 == linlin( 1.0 ) );
        CHECK( 7.0 == linlin( 5.0 ) );
        CHECK( 5.5 == linlin( 8.0 ) );

        // Somewhere inbetween
        CHECK( linlinInterpolation( 1.5, 1, 5, 2, 7 ) == linlin( 1.5 ) );
        CHECK( linlinInterpolation( 5.5, 5, 8, 7, 5.5 ) == linlin( 5.5 ) );
      } // THEN
    } // WHEN
    WHEN( "creating a LinearLogarithmic interpolation table (LAW=3)" ){
      interp::LinearLogarithmic linlog{ 
          njoy::utility::copy( x1 ), njoy::utility::copy( y1 ) };

      THEN( "we can verify interpolated values" ){
        // Given data
        CHECK( 2.0 == linlog( 1.0 ) );
        CHECK( 7.0 == linlog( 5.0 ) );
        CHECK( 5.5 == linlog( 8.0 ) );

        // Somewhere inbetween
        CHECK( linlogInterpolation( 1.5, 1, 5, 2, 7 ) == linlog( 1.5 ) );
        CHECK( linlogInterpolation( 5.5, 5, 8, 7, 5.5 ) == linlog( 5.5 ) );
      } // THEN
    } // WHEN
    WHEN( "creating a LogarithmicLinear interpolation table (LAW=4)" ){
      interp::LogarithmicLinear loglin{ 
          njoy::utility::copy( x1 ), njoy::utility::copy( y1 ) };

      THEN( "we can verify interpolated values" ){
        // Given data
        CHECK( 2.0 == loglin( 1.0 ) );
        CHECK( 7.0 == loglin( 5.0 ) );
        CHECK( 5.5 == loglin( 8.0 ) );

        // Somewhere inbetween
        CHECK( loglinInterpolation( 1.5, 1, 5, 2, 7 ) == loglin( 1.5 ) );
        CHECK( loglinInterpolation( 5.5, 5, 8, 7, 5.5 ) == loglin( 5.5 ) );
      } // THEN
    } // WHEN
    WHEN( "creating a LogarithmicLogarithmic interpolation table (LAW=5)" ){
      interp::LogarithmicLogarithmic loglog{ 
          njoy::utility::copy( x1 ), njoy::utility::copy( y1 ) };

      THEN( "we can verify interpolated values" ){
        // Given data
        CHECK( 2.0 == loglog( 1.0 ) );
        CHECK( 7.0 == loglog( 5.0 ) );
        CHECK( 5.5 == loglog( 8.0 ) );

        // Somewhere inbetween
        CHECK( loglogInterpolation( 1.5, 1, 5, 2, 7 ) == loglog( 1.5 ) );
        CHECK( loglogInterpolation( 5.5, 5, 8, 7, 5.5 ) == loglog( 5.5 ) );
      } // THEN
    } // WHEN
  } // GIVEN
} // SCENARIO
