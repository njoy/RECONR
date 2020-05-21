#include "catch.hpp"

#include "RECONR.hpp"

#include "RECONR/details/simpleENDFTestString.hpp"

SCENARIO( "Testing the collection of resonance parameter data" ){

  GIVEN( "ENDF Material with SLBW" ){
    auto material = details::ENDFMaterial( "SLBW" );

    WHEN( "the resonance parameters are extracted" ){
      auto isos = std::get< 0 >(
        njoy::RECONR::R2D2::Factory::collectRP( material ) );

      THEN( "we can verify a few things" ){
        // Note, I'm not testing everything because I trust ENDFtk to do it's
        // job
        CHECK( 1 == ranges::distance( isos ) );
      } // THEN
    } // WHEN
    
  } // GIVEN
  
} // SCENARIO
