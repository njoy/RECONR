#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "RECONR.hpp"

#include "RECONR/details/simpleENDFTestString.hpp"
#include "RECONR/details/interpLambdas.hpp"
#include "RECONR/details/checkRanges.hpp"

SCENARIO( "Testing the collection of resonance parameter data" ){

  GIVEN( "ENDF Material with SLBW" ){
    auto material = details::ENDFMaterial( "SLBW" );

    WHEN( "the resonance parameters are extracted" ){
      auto resParams = std::get< 0 >(
        njoy::RECONR::R2D2::Factory::collectRP( material ) );

      THEN( "we can verify a few things" ){

        CHECK( 4.5105E4 == resParams.ZA() );
        CHECK( 104.0 == resParams.atomicWeightRatio() );

        auto isos = resParams.isotopes();
        // Note, I'm not testing everything because I trust ENDFtk to do it's
        // job
        CHECK( 1 == ranges::distance( isos ) );
      } // THEN
    } // WHEN
    
  } // GIVEN
  
} // SCENARIO

SCENARIO( "Testing the the collection of cross sections" ){
  GIVEN( "an ENDF Material" ){
    auto material = details::ENDFMaterial( "SLBW", false );

    WHEN( "the resonance reconstruction data can be extracted" ){
      auto reactions = njoy::RECONR::R2D2::Factory::collectXS( material );

      std::vector< int > MTs{ 1, 2, 16, 18, 51, 52, 102, 875, 876, 877 };
      CHECK( ranges::equal( MTs, ranges::view::keys( reactions ) ) );

      THEN( "MT=1 can be checked" ){
        auto reaction = reactions.at( 1 );

        CHECK( 1001 == reaction.ZA() );
        CHECK( 0.9991673 == reaction.AWR() );
        CHECK( 2.224648E6 == reaction.QM() );
        CHECK( 3.224648E6 == reaction.QI() );
        CHECK( 0 == reaction.LR() );

        auto xs = std::get< njoy::RECONR::interp::Histogram >( 
            reaction.crossSections()[ 0 ] );
        std::vector< double > refE{
          1.0E-5, 2.0E-5, 7.5E+5, 1.9E+7, 1.95E+7, 2.0E+7 };
        std::vector< double > refB{
          1.0, 1.182897E1, 3.347392E-5, 2.751761E-5, 2.731301E-5, 2.710792E-5 };
      
        auto energies = xs.x() | ranges::to_vector;
        auto barns = xs.y() | ranges::to_vector;
      
        details::checkRanges( refE, energies );
        details::checkRanges( refB, barns );

        CHECK( 1.0 == Approx( xs( 1.5E-5 ) ) );
        CHECK( 1.0 == Approx( xs( 1.999999999E-5 ) ) );
        CHECK( 1.1828971E1 == Approx( xs( 2.0E-5 ) ) );
        CHECK( 1.1828971E1 == Approx( xs( 1 ) ) );
        CHECK( 3.347392E-5 == Approx( xs( 8E5 ) ) );
        CHECK( 2.751761E-5 == Approx( xs( 1.925E7 ) ) );
        CHECK( 2.731301E-5 == Approx( xs( 1.975E7 ) ) );
      } // THEN
      THEN( "MT=2 can be checked" ){ 
        auto reaction = reactions.at( 2 );

        CHECK( 1002 == reaction.ZA() );
        CHECK( 0.9991673 == reaction.AWR() );
        CHECK( 2.224648E2 == reaction.QM() );
        CHECK( 3.224648E2 == reaction.QI() );
        CHECK( 0 == reaction.LR() );

        auto xs = std::get< njoy::RECONR::interp::Histogram >( 
            reaction.crossSections()[ 0 ] );
        std::vector< double > refE{
          1.0E-5, 2.0E-5, 7.5E+5, 1.9E+7, 1.95E+7, 2.0E+7 };
        std::vector< double > refB( refE.size(), 2.0 );

        auto energies = xs.x() | ranges::to_vector;
        auto barns = xs.y() | ranges::to_vector;

        details::checkRanges( refE, energies );
        details::checkRanges( refB, barns );

        CHECK( 2.0 == Approx( xs( 1.5E-5 ) ) );
        CHECK( 2.0 == Approx( xs( 1.999999999E-5 ) ) );
        CHECK( 2.0 == Approx( xs( 2.0E-5 ) ) );
        CHECK( 2.0 == Approx( xs( 1 ) ) );
        CHECK( 2.0 == Approx( xs( 8E5 ) ) );
        CHECK( 2.0 == Approx( xs( 1.925E7 ) ) );
        CHECK( 2.0 == Approx( xs( 1.975E7 ) ) );
      }
      THEN( "MT=16 can be checked" ){ 
        auto reaction = reactions.at( 16 );

        CHECK( 1016 == reaction.ZA() );
        CHECK( 0.9991673 == reaction.AWR() );
        CHECK( 2.22464E16 == reaction.QM() );
        CHECK( 3.22464E16 == reaction.QI() );
        CHECK( 0 == reaction.LR() );

        auto xs = std::get< njoy::RECONR::interp::LogarithmicLogarithmic >( 
            reaction.crossSections()[ 0 ] );
        std::vector< double > refE{
          1.0E-5, 2.0E-5, 7.5E+5, 1.9E+7, 1.95E+7, 2.0E+7 };
        std::vector< double > refB{
          1.6E1, 1.182897E1, 3.347392E-5, 
          2.751761E-5, 2.731301E-5, 2.710792E-5 };

        auto energies = xs.x() | ranges::to_vector;
        auto barns = xs.y() | ranges::to_vector;

        details::checkRanges( refE, energies );
        details::checkRanges( refB, barns );

        double refY = loglogInterpolation( 
          1.5E-5, refE[ 0 ], refE[ 1 ], refB[ 0 ], refB[ 1 ] );
        CHECK( refY == Approx( xs( 1.5E-5 ) ) );

        refY = loglogInterpolation( 
          1, refE[ 1 ], refE[ 2 ], refB[ 1 ], refB[ 2 ] );
        CHECK( refY == Approx( xs( 1 ) ) );

        refY = loglogInterpolation( 
          8E5, refE[ 2 ], refE[ 3 ], refB[ 2 ], refB[ 3 ] );
        CHECK( refY == Approx( xs( 8E5 ) ) );

        refY = loglogInterpolation( 
          1.925E7, refE[ 3 ], refE[ 4 ], refB[ 3 ], refB[ 4 ] );
        CHECK( refY == Approx( xs( 1.925E7 ) ) );

        refY = loglogInterpolation( 
          1.975E7, refE[ 4 ], refE[ 5 ], refB[ 4 ], refB[ 5 ] );
        CHECK( refY == Approx( xs( 1.975E7 ) ) );
      }
      THEN( "MT=18 can be checked" ){ 
        auto reaction = reactions.at( 18 );

        CHECK( 1018 == reaction.ZA() );
        CHECK( 0.9991673 == reaction.AWR() );
        CHECK( 2.22464E18 == reaction.QM() );
        CHECK( 3.22464E18 == reaction.QI() );
        CHECK( 0 == reaction.LR() );

        {
          auto xs1 = std::get< njoy::RECONR::interp::Histogram >(
              reaction.crossSections()[ 0 ] );
          std::vector< double > refE{ 1.0E+5, 1.5E+5, 7.5E+5 };
          std::vector< double > refB{ 1.8E+1, 1.182897E+1, 3.347392E-5 };

          auto energies = xs1.x() | ranges::to_vector;
          auto barns = xs1.y() | ranges::to_vector;

          details::checkRanges( refE, energies );
          details::checkRanges( refB, barns );

          CHECK( 18.0 == Approx( xs1( 1.25E+5 ) ) );
          CHECK( 1.182897E1 == Approx( xs1( 2E+5 ) ) );
          CHECK( 3.347392E-5 == Approx( xs1( 7.5E5 ) ).margin( 1E-7 ) );
        }
        {
          auto xs2 = std::get< njoy::RECONR::interp::LinearLinear >(
              reaction.crossSections()[ 1 ] );
          std::vector< double > refE{ 7.5E+5, 1.9E+7, 1.95E+7, 2.0E+7 };
          std::vector< double > refB{ 
            3.347392E-5, 2.751761E-5, 2.731301E-5, 2.710792E-5 };

          auto energies = xs2.x() | ranges::to_vector;
          auto barns = xs2.y() | ranges::to_vector;

          details::checkRanges( refE, energies );
          details::checkRanges( refB, barns );

          auto refY = linlinInterpolation( 
            1.925E7, refE[ 0 ], refE[ 1 ], refB[ 0 ], refB[ 1 ] );
          CHECK( refY == Approx( xs2( 1.925E7 ) ) );

          refY = linlinInterpolation( 
            1.975E7, refE[ 1 ], refE[ 2 ], refB[ 1 ], refB[ 2 ] );
          CHECK( refY == Approx( xs2( 1.975E7 ) ) );
        }
      }
      THEN( "MT=102 can be checked" ){ 
        auto reaction = reactions.at( 102 );

        CHECK( 1102 == reaction.ZA() );
        CHECK( 0.9991673 == reaction.AWR() );
        CHECK( 2.224648E6 == reaction.QM() );
        CHECK( 3.224648E6 == reaction.QI() );
        CHECK( 0 == reaction.LR() );

        {
          auto xs102 = std::get< njoy::RECONR::interp::LinearLogarithmic >( 
              reaction.crossSections()[ 0 ] );
          std::vector< double > refE{ 1.0E-5, 2.0E-5, 7.5E+5 };
          std::vector< double > refB{ 1.02E+2, 1.182897E+1, 3.347392E-5 };

          auto energies = xs102.x() | ranges::to_vector;
          auto barns = xs102.y() | ranges::to_vector;

          details::checkRanges( refE, energies );
          details::checkRanges( refB, barns );

          double refY = linlogInterpolation( 
            1.5E-5, refE[ 0 ], refE[ 1 ], refB[ 0 ], refB[ 1 ] );
          CHECK( refY == Approx( xs102( 1.5E-5 ) ).margin( 1E-7 ) );

          refY = linlogInterpolation( 
            1, refE[ 1 ], refE[ 2 ], refB[ 1 ], refB[ 2 ] );
          CHECK( refY == Approx( xs102( 1 ) ).margin( 1E-7 ) );
        }
        {
          auto xs4 = std::get< njoy::RECONR::interp::LogarithmicLinear >( 
              reaction.crossSections()[ 1 ] );
          std::vector< double > refE{ 7.5E+5, 1.9E+7, 1.95E+7, 2.0E+7 };
          std::vector< double > refB{
             3.347392E-5, 2.751761E-5, 2.731301E-5, 2.710792E-5 };

          auto energies = xs4.x() | ranges::to_vector;
          auto barns = xs4.y() | ranges::to_vector;

          details::checkRanges( refE, energies );
          details::checkRanges( refB, barns );

          auto refY = loglinInterpolation( 
            8E5, refE[ 0 ], refE[ 1 ], refB[ 0 ], refB[ 1 ] );
          CHECK( refY == Approx( xs4( 8E5 ) ).margin( 1E-7 ) );

          refY = loglinInterpolation( 
            1.925E7, refE[ 1 ], refE[ 2 ], refB[ 1 ], refB[ 2 ] );
          CHECK( refY == Approx( xs4( 1.925E7 ) ).margin( 1E-7 ) );

          refY = loglinInterpolation( 
            1.975E7, refE[ 2 ], refE[ 3 ], refB[ 2 ], refB[ 3 ] );
          CHECK( refY == Approx( xs4( 1.975E7 ) ).margin( 1E-7 ) );
        }
      }
    } // WHEN
  } // GIVEN
} // SCENARIO

SCENARIO( "Testing the collection of photon production cross sections" ){
  GIVEN( "an ENDF Material" ){
    auto material = details::ENDFMaterial( "SLBW", true );

    WHEN( "the photon production cross sections have been extracted" ){
      auto reactions = njoy::RECONR::R2D2::Factory::collectPPXS( material );
      
      std::vector< int > MTs{ 3, 18 };
      auto keys = ranges::view::keys( reactions );
      njoy::Log::info( "keys: {}", keys | ranges::view::all );
      CHECK( ranges::equal( MTs, keys ) );
    } // WHEN
    
  } // GIVEN
  
} // SCENARIO
