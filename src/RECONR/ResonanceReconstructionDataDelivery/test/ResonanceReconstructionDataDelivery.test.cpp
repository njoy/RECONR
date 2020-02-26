#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "RECONR.hpp"

#include "RECONR/details/simpleENDFTestString.hpp"

SCENARIO( "Testing R2D2 capabilities" ){
  GIVEN( "an an R2D2" ){
    njoy::RECONR::R2D2 r2d2{};

    THEN( "a cross section table can be added" ){
      std::vector< double > refEnergies{ 1, 2, 3 };
      std::vector< double > refBarns{ 4, 5, 6 };
      std::vector< njoy::RECONR::interp::Variant > cs;
      cs.emplace_back( 
        njoy::RECONR::interp::Variant( 
            njoy::RECONR::interp::LinearLinear( 
                njoy::utility::copy( refEnergies ), 
                njoy::utility::copy( refBarns ) ) ) );

      CHECK_NOTHROW( r2d2.emplace( 1, std::move( cs ) ) );

      AND_THEN( "the added cross section can be retrieved" ){
        auto xs = r2d2.at( 1 );
        auto energies = xs.x() | ranges::to_vector;
        auto barns = xs.y() | ranges::to_vector;

        CHECK( ranges::distance( refEnergies ) == ranges::distance( energies ) );
        CHECK( ranges::distance( refBarns ) == ranges::distance( barns ) );
        CHECK( ranges::equal( refEnergies, energies ) );
        CHECK( ranges::equal( refBarns, barns ) );
      } // AND_THEN
    } // THEN
  } // GIVEN
  
} // SCENARIO
