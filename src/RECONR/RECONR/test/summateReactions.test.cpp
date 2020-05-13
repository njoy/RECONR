#include "catch.hpp"

#include "RECONR.hpp"
#include "RECONR/details/simpleENDFTestString.hpp"

/* Linearize and reconstruct resonances
 */
auto lin_recon( std::string formalism, double absTol, double relTol ){
  std::vector< double > userSupplied{ 1.0, 2.0, 3.0 };
  auto material = details::ENDFMaterial( formalism );
  auto r2d2 = njoy::RECONR::R2D2::Factory( std::move( material ) )();

  njoy::RECONR::RECONR::linearizeXS( r2d2, absTol, relTol);
  auto refGrid = njoy::RECONR::RECONR::unionizeEnergyGrid( r2d2, userSupplied );
  njoy::RECONR::RECONR::reconstructResonances( refGrid, r2d2, relTol, absTol );
  auto energies = njoy::RECONR::RECONR::unionizeEnergyGrid( r2d2 );

  return std::make_pair( energies, r2d2 );
}

SCENARIO( "Testing the summation of cross sections" ){
  double absTol{ 1E-6 };
  double relTol{ 1E-1 }; // This tolerance is large by design

  GIVEN( "an SLBW object" ){
    WHEN( "cross sections have been linearized and resonances reconstructed" ){
      auto [energies, r2d2] = lin_recon( "SLBW", absTol, relTol );

      THEN( "the reactions can be summed up" ){

        auto reactions = njoy::RECONR::RECONR::summateReactions( 
            r2d2, energies );
        
      } // THEN
    } // WHEN
  } // GIVEN
  
} // SCENARIO

