#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "RECONR.hpp"

using namespace njoy::RECONR::interp;

SCENARIO( "Testing interpolation capabilities" ){
  GIVEN("an x- and y-grid"){
    std::vector< double > xGrid1{1.0, 2.0, 2.0, 3.0};
    std::vector< double > yGrid1{3.0, 5.0, 6.0, 7.0};
    std::vector< double > xGrid2{3.0, 4.0, 5.0, 6.0};
    std::vector< double > yGrid2{4.0, 6.0, 7.0, 8.0};

    WHEN( "an interpolation table is created" ){

      std::vector< LinearLinear > vTable{
        LinearLinear{ std::move( xGrid1 ), std::move( yGrid1 ) },
        LinearLinear{ std::move( xGrid2 ), std::move( yGrid2 ) }
      };

      LinearTable table{ std::move( vTable ) };

      THEN( "the interpolation can be verified" ){
        CHECK( 0.0 == table( 0.5 ) );
        CHECK( 0.0 == table( 6.5 ) );
      } // THEN
      
    } // WHEN
  } // GIVEN
} // SCENARIO
