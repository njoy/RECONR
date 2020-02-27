#include "catch.hpp"

#include "RECONR.hpp"

#include "RECONR/details/simpleENDFTestString.hpp"

njoy::RECONR::R2D2::ENDFMaterial_t ENDFMaterial( std::string );

SCENARIO( "Testing the linearization of collected cross sections" ){
  GIVEN( "an ResonanceReconstructionDataDelivery object" ){
    auto material = ENDFMaterial( "SLBW" );
    auto r2d2 = njoy::RECONR::R2D2::Factory( std::move( material ) )();

    THEN( "the cross sections can be linearized" ){
      auto linear = njoy::RECONR::RECONR::linearize( r2d2, 1E-6, 1E-4 );

      auto rXSs = r2d2.crossSections();
      auto lXSs = linear.crossSections();

      auto keys = ranges::view::keys( rXSs );
      CHECK( ranges::equal( keys, ranges::view::keys( lXSs ) ) );

      {
        int MT{ 1 };
        auto rxs = rXSs.at( MT );
        auto lxs = lXSs.at( MT );

        auto refX = rxs.x() | ranges::to_vector;
        auto linX = lxs.x() | ranges::to_vector;
        njoy::Log::info( "refX: {}", refX | ranges::view::all );
        njoy::Log::info( "linX: {}", linX | ranges::view::all );
        CHECK( ranges::equal( refX, linX ) );
      }
    } // THEN
  } // GIVEN
} // SCENARIO
