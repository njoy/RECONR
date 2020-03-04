#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "RECONR.hpp"

auto loglogInterpolation = []( double x, 
                               double x1, double x2, 
                               double y1, double y2 ){
  const auto inverseX1 = 1.0 / x1;
  const auto logRatio = std::log( y2/y1 )/std::log( x2 * inverseX1 );
  return y1*std::pow( x*inverseX1, logRatio );
};
auto linlinInterpolation = []( double x, 
                               double x1, double x2, 
                               double y1, double y2 ){
  return y1 + ( y2 - y1) * ( x - x1 ) / ( x2 - x1 );
};
auto linlogInterpolation = []( double x, 
                               double x1, double x2, 
                               double y1, double y2 ){
  const auto inverseX1 = 1.0 / x1;
  return y1 + ( y2 - y1 )*
    std::log( x*inverseX1 )/std::log( x2*inverseX1 );
};
auto loglinInterpolation = []( double x, 
                               double x1, double x2, 
                               double y1, double y2 ){
  return y1*std::pow( ( y2/y1 ), ( x - x1 )/( x2 - x1 ) );
};
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

        // Somewhere inbetween
        CHECK( linlinInterpolation( 1.5, 1, 5, 2, 7 ) == table( 1.5 ) );
        CHECK( linlinInterpolation( 5.5, 5, 8, 7, 5.5 ) == table( 5.5 ) );
        CHECK( linlinInterpolation( 8.2, 8, 10, 2.2, 7.2 ) == table( 8.2 ) );
        CHECK( linlinInterpolation( 12.0, 10, 18, 7.2, 5.7 ) == table( 12.0 ) );

        // CHECK_THROWS( table( 19.0 ) );
      } // THEN
    } // WHEN
    WHEN( "creating a LinearLogarithmic interpolation table (LAW=3)" ){
      XS.emplace_back( njoy::RECONR::interp::Variant(
        njoy::RECONR::interp::LinearLogarithmic( 
            njoy::utility::copy( x1 ), njoy::utility::copy( y1 ) ) ) );
      XS.emplace_back( njoy::RECONR::interp::Variant(
        njoy::RECONR::interp::LinearLogarithmic( 
            njoy::utility::copy( x2 ), njoy::utility::copy( y2 ) ) ) );
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
        CHECK( linlogInterpolation( 1.5, 1, 5, 2, 7 ) == table( 1.5 ) );
        CHECK( linlogInterpolation( 5.5, 5, 8, 7, 5.5 ) == table( 5.5 ) );
        CHECK( linlogInterpolation( 8.2, 8, 10, 2.2, 7.2 ) == table( 8.2 ) );
        CHECK( linlogInterpolation( 12.0, 10, 18, 7.2, 5.7 ) == table( 12.0 ) );

        // CHECK_THROWS( table( 19.0 ) );
      } // THEN
    } // WHEN
    WHEN( "creating a LogarithmicLinear interpolation table (LAW=4)" ){
      XS.emplace_back( njoy::RECONR::interp::Variant(
        njoy::RECONR::interp::LogarithmicLinear( 
            njoy::utility::copy( x1 ), njoy::utility::copy( y1 ) ) ) );
      XS.emplace_back( njoy::RECONR::interp::Variant(
        njoy::RECONR::interp::LogarithmicLinear( 
            njoy::utility::copy( x2 ), njoy::utility::copy( y2 ) ) ) );
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
        CHECK( loglinInterpolation( 1.5, 1, 5, 2, 7 ) == table( 1.5 ) );
        CHECK( loglinInterpolation( 5.5, 5, 8, 7, 5.5 ) == table( 5.5 ) );
        CHECK( loglinInterpolation( 8.2, 8, 10, 2.2, 7.2 ) == table( 8.2 ) );
        CHECK( loglinInterpolation( 12.0, 10, 18, 7.2, 5.7 ) == table( 12.0 ) );

        // CHECK_THROWS( table( 19.0 ) );
      } // THEN
    } // WHEN
    WHEN( "creating a LogarithmicLogarithmic interpolation table (LAW=5)" ){
      XS.emplace_back( njoy::RECONR::interp::Variant(
        njoy::RECONR::interp::LogarithmicLogarithmic( 
            njoy::utility::copy( x1 ), njoy::utility::copy( y1 ) ) ) );
      XS.emplace_back( njoy::RECONR::interp::Variant(
        njoy::RECONR::interp::LogarithmicLogarithmic( 
            njoy::utility::copy( x2 ), njoy::utility::copy( y2 ) ) ) );
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
        CHECK( loglogInterpolation( 1.5, 1, 5, 2, 7 ) == table( 1.5 ) );
        CHECK( loglogInterpolation( 5.5, 5, 8, 7, 5.5 ) == table( 5.5 ) );
        CHECK( loglogInterpolation( 8.2, 8, 10, 2.2, 7.2 ) == table( 8.2 ) );
        CHECK( loglogInterpolation( 12.0, 10, 18, 7.2, 5.7 ) == table( 12.0 ) );

        // CHECK_THROWS( table( 19.0 ) );
      } // THEN
    } // WHEN
  } // GIVEN
  
} // SCENARIO
