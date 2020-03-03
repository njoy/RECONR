#include "catch.hpp"

#include "RECONR.hpp"

#include "RECONR/details/simpleENDFTestString.hpp"

njoy::RECONR::R2D2::ENDFMaterial_t ENDFMaterial( std::string );

SCENARIO( "Testing the linearization of collected cross sections" ){
  GIVEN( "an ResonanceReconstructionDataDelivery object" ){
    auto material = ENDFMaterial( "SLBW" );
    auto r2d2 = njoy::RECONR::R2D2::Factory( std::move( material ) )();

    THEN( "the cross sections can be linearized" ){
      auto linear = njoy::RECONR::RECONR::linearize( r2d2, 1E-6, 1E-3 );

      auto rXSs = r2d2.crossSections();
      auto lXSs = linear.crossSections();

      auto keys = ranges::view::keys( rXSs );
      CHECK( ranges::equal( keys, ranges::view::keys( lXSs ) ) );
      
      for( auto MT: keys ){
        njoy::Log::info( "MT: {}", MT );
        auto rxs = rXSs.at( MT );
        auto lxs = lXSs.at( MT );

        auto refX = rxs.x() | ranges::to_vector;
        auto refY = rxs.y() | ranges::to_vector;
        auto linX = lxs.x() | ranges::to_vector;
        auto linY = lxs.y() | ranges::to_vector;
        auto oY = lxs.x() | ranges::view::transform( rxs ) | ranges::to_vector;
        // njoy::Log::info( "{}, refX: {}", MT, refX | ranges::view::all );
        // njoy::Log::info( "{}, refY: {}", MT, refY | ranges::view::all );
        // njoy::Log::info( "{}, linX: {}", MT, linX | ranges::view::all );
        // njoy::Log::info( "{}, linY: {}", MT, linY | ranges::view::all );
        // njoy::Log::info( "{}, oY: {}", MT, oY | ranges::view::all );
        // CHECK( ranges::equal( refX, linX ) );
      }



    } // THEN
  } // GIVEN
} // SCENARIO
