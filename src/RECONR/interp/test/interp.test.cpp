#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "RECONR.hpp"

SCENARIO( "Testing interpolation 'LAWS'." ){
  GIVEN( "known (x,y) values" ){
    std::vector< double > x1{ 1.0, 5.0, 8.0 };
    std::vector< double > y1{ 2.0, 7.0, 5.5 };
    std::vector< double > x2{ 8.0, 10.0, 18.0 };
    std::vector< double > y2{ 2.2, 7.2, 5.7 };

    std::vector< njoy::RECONR::interp::Variant > XS;

    WHEN( "creating a Histogram interpolation table (LAW=1)" ){
      XS.emplace_back( njoy::RECONR::interp::Variant(
        njoy::RECONR::interp::Histogram( njoy::utility::copy( x1 ),
                                         njoy::utility::copy( y1 ) ) ) );
      XS.emplace_back( njoy::RECONR::interp::Variant(
        njoy::RECONR::interp::Histogram( njoy::utility::copy( x2 ),
                                         njoy::utility::copy( y2 ) ) ) );
      auto table = njoy::RECONR::interp::Table( std::move( XS ) );

      THEN( "we can verify interpolated values" ){
        // Given data
        CHECK( 2.0 == table( 1.0 ) );
        CHECK( 7.0 == table( 5.0 ) );
        CHECK( 5.5 == table( 8.0 ) );
        CHECK( 5.5 == table( 8.0 ) );
        CHECK( 7.2 == table( 10.0 ) );
        CHECK( 5.7 == table( 18.0 ) );

        // Somewhere inbetween
        CHECK( 2.0 == table( 1.5 ) );
        CHECK( 7.0 == table( 5.5 ) );
        CHECK( 2.2 == table( 8.2 ) );
        CHECK( 7.2 == table( 12.0 ) );

        // CHECK_THROWS( table( 19.0 ) );
      } // THEN
    } // WHEN
    WHEN( "creating a LinearLinear interpolation table (LAW=2)" ){
      XS.emplace_back( njoy::RECONR::interp::Variant(
        njoy::RECONR::interp::LinearLinear( njoy::utility::copy( x1 ),
                                            njoy::utility::copy( y1 ) ) ) );
      XS.emplace_back( njoy::RECONR::interp::Variant(
        njoy::RECONR::interp::LinearLinear( njoy::utility::copy( x2 ),
                                            njoy::utility::copy( y2 ) ) ) );
      auto table = njoy::RECONR::interp::Table( std::move( XS ) );

      THEN( "we can verify interpolated values" ){
        // Given data
        CHECK( 2.0 == table( 1.0 ) );
        CHECK( 7.0 == table( 5.0 ) );
        CHECK( 5.5 == table( 8.0 ) );
        CHECK( 5.5 == table( 8.0 ) );
        CHECK( 7.2 == table( 10.0 ) );
        CHECK( 5.7 == table( 18.0 ) );

        auto linlinInterpolation = []( double x, 
                                       double x1, double x2, 
                                       double y1, double y2 ){
          return y1 + ( y2 - y1) * ( x - x1 ) / ( x2 - x1 );
        };
        // Somewhere inbetween
        CHECK( linlinInterpolation( 1.5, 1, 5, 2, 7 ) == table( 1.5 ) );
        CHECK( linlinInterpolation( 5.5, 5, 8, 7, 5.5 ) == table( 5.5 ) );
        CHECK( linlinInterpolation( 8.2, 8, 10, 2.2, 7.2 ) == table( 8.2 ) );
        CHECK( linlinInterpolation( 12.0, 10, 18, 7.2, 5.7 ) == table( 12.0 ) );

        // CHECK_THROWS( table( 19.0 ) );
      } // THEN
    } // WHEN
  } // GIVEN
  
} // SCENARIO
