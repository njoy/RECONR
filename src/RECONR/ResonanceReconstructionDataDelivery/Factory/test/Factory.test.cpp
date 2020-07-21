#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "RECONR.hpp"

#include "RECONR/details/simpleENDFTestString.hpp"
#include "RECONR/details/interpLambdas.hpp"
#include "RECONR/details/checkRanges.hpp"
#include "RECONR/details/printV.hpp"

using namespace njoy;
using namespace njoy::RECONR;
using namespace njoy::elementary;

class TFactory: protected njoy::RECONR::R2D2::Factory {
  using Factory = njoy::RECONR::R2D2::Factory;

public:
  using Factory::Factory;
  using Factory::collectRP;
  using Factory::collectXS;
  using Factory::collectPPXS;
};

SCENARIO( "Testing the collection of resonance parameter data" ){

  GIVEN( "ENDF Material with SLBW" ){
    auto material = details::ENDFMaterial( "SLBW" );

    WHEN( "the resonance parameters are extracted" ){
      auto resParams = std::get< 0 >( TFactory::collectRP( material ) );

      THEN( "we can verify a few things" ){

        CHECK( 4.5105E4 == resParams.ZA() );
        CHECK( 104.0 == resParams.atomicWeightRatio() );

        auto isos = resParams.isotopes();
        // Note, I'm not testing everything because I trust ENDFtk to do its
        // job
        CHECK( 1 == ranges::distance( isos ) );
      } // THEN
    } // WHEN
    
  } // GIVEN
  GIVEN( "ENDF Material with RM" ){
    auto material = details::ENDFMaterial( "RM" );

    WHEN( "the resonance parameters are extracted" ){
      auto resParams = std::get< 0 >( TFactory::collectRP( material ) );

      THEN( "we can verify a few things" ){

        CHECK( 4.5105E4 == resParams.ZA() );
        CHECK( 104.0 == resParams.atomicWeightRatio() );

        auto isos = resParams.isotopes();
        // Note, I'm not testing everything because I trust ENDFtk to do its
        // job
        CHECK( 1 == ranges::distance( isos ) );
      } // THEN
    } // WHEN
    
  } // GIVEN
  GIVEN( "ENDF Material with RML" ){
    auto material = details::ENDFMaterial( "RML" );

    WHEN( "the resonance parameters are extracted" ){
      auto resParams = std::get< 0 >( TFactory::collectRP( material ) );

      THEN( "we can verify a few things" ){

        CHECK( 4.5105E4 == resParams.ZA() );
        CHECK( 104.0 == resParams.atomicWeightRatio() );

        auto isos = resParams.isotopes();
        // Note, I'm not testing everything because I trust ENDFtk to do its
        // job
        CHECK( 1 == ranges::distance( isos ) );
      } // THEN
    } // WHEN
    
  } // GIVEN
  
} // SCENARIO

SCENARIO( "Testing the the collection of cross sections" ){
  GIVEN( "an ENDF Material" ){
    auto material = details::ENDFMaterial( "SLBW", false );

    WHEN( "the resonance reconstruction data is extracted" ){
      
      ParticleID projectile( "n" );
      ParticleID target( "h1" );
      auto reactions = TFactory::collectXS( material, projectile, target );

      using PID = ParticleID;
      using NID = NucleusID;

      std::vector< njoy::RECONR::ReactionID > IDs{ 
        // ReactionID{ PID{ "n" }, PID{ NID{ 1001, 0 } }, ReactionType{ 1 } },
        ReactionID{ PID{ "n" }, PID{ NID{ 1001, 0 } }, ReactionType{ 2 } },
        // ReactionID{ PID{ "n" }, PID{ NID{ 1001, 0 } }, ReactionType{ 16 } },
        ReactionID{ PID{ "n" }, PID{ NID{ 1001, 0 } }, ReactionType{ 18 } },
        ReactionID{ PID{ "n" }, PID{ NID{ 1001, 0 } }, ReactionType{ 51 } },
        ReactionID{ PID{ "n" }, PID{ NID{ 1001, 0 } }, ReactionType{ 52 } },
        ReactionID{ PID{ "n" }, PID{ NID{ 1001, 0 } }, ReactionType{ 102 } },
        ReactionID{ PID{ "n" }, PID{ NID{ 1001, 0 } }, ReactionType{ 875 } },
        ReactionID{ PID{ "n" }, PID{ NID{ 1001, 0 } }, ReactionType{ 876 } },
        ReactionID{ PID{ "n" }, PID{ NID{ 1001, 0 } }, ReactionType{ 877 } }
      };
      auto keys = ranges::view::keys( reactions ) | ranges::to_vector;
      std::sort( IDs.begin(), IDs.end() );
      std::sort( keys.begin(), keys.end() );

      CHECK( ranges::equal( IDs, keys ) );

      for( auto id : IDs ){
        THEN( "ID: " + id.symbol() + " should be in map" ){
          CHECK( 1 == reactions.count( id ) );
          CHECK_NOTHROW( reactions.at( id ) );
        } // THEN
      }

      THEN( "MT=2 can be checked" ){ 
        ReactionID reactionID{ PID{ "n" }, PID{ "h1" }, ReactionType{ 2 } };
        auto reaction = reactions.at( reactionID );

        CHECK( 1002 == reaction.ZA() );
        CHECK( 0.9991673 == reaction.AWR() );
        CHECK( 2.224648E2 == reaction.QM() );
        CHECK( 3.224648E2 == reaction.QI() );
        CHECK( 0 == reaction.LR() );

        auto xs = std::get< njoy::RECONR::interp::Histogram >( 
            reaction.crossSections< std::vector< interp::Variant > >()[ 0 ] );
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
      THEN( "MT=18 can be checked" ){ 
        ReactionID reactionID{ PID{ "n" }, PID{ "h1" }, ReactionType{ 18 } };
        auto reaction = reactions.at( reactionID );

        CHECK( 1018 == reaction.ZA() );
        CHECK( 0.9991673 == reaction.AWR() );
        CHECK( 2.224648E2 == reaction.QM() );
        CHECK( 3.224648E2 == reaction.QI() );
        CHECK( 0 == reaction.LR() );

        {
          auto xs1 = std::get< njoy::RECONR::interp::Histogram >(
              reaction.crossSections< std::vector< interp::Variant > >()[ 0 ] );
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
              reaction.crossSections< std::vector< interp::Variant > >()[ 1 ] );
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
        ReactionID reactionID{ PID{ "n" }, PID{ "h1" }, ReactionType{ 102 } };
        auto reaction = reactions.at( reactionID );

        CHECK( 1102 == reaction.ZA() );
        CHECK( 0.9991673 == reaction.AWR() );
        CHECK( 2.224648E6 == reaction.QM() );
        CHECK( 3.224648E6 == reaction.QI() );
        CHECK( 0 == reaction.LR() );

        {
          auto xs102 = std::get< njoy::RECONR::interp::LinearLogarithmic >( 
              reaction.crossSections< std::vector< interp::Variant > >()[ 0 ] );
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
              reaction.crossSections< std::vector< interp::Variant > >()[ 1 ] );
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
    auto material = details::ENDFMaterial( "SLBW", false );

    WHEN( "the photon production cross sections have been extracted" ){
      njoy::elementary::ParticleID projectile( "n" );
      njoy::elementary::ParticleID target( "h1" );
      auto reactions = TFactory::collectPPXS( material, projectile, target );
      
      using PID = ParticleID;
      using NID = NucleusID;

      std::vector< njoy::RECONR::ReactionID > IDs{ 
        ReactionID{ PID{ "n" }, PID{ NID{ 1001, 0 } }, ReactionType{ 3 } },
        ReactionID{ PID{ "n" }, PID{ NID{ 1001, 0 } }, ReactionType{ 18 } },
      };
      auto keys = ranges::view::keys( reactions ) | ranges::to_vector;
      std::sort( IDs.begin(), IDs.end() );
      std::sort( keys.begin(), keys.end() );

      CHECK( ranges::equal( IDs, keys ) );

      THEN( "MT=3 can be checked" ){

        ReactionID reactionID{ PID{ "n" }, PID{ "h1" }, ReactionType{ 3 } };
        auto reaction = reactions.at( reactionID );

        CHECK( 92235 == reaction.ZA() );
        CHECK( 233.0248 == reaction.atomicWeightRatio() );

        std::vector< double > refE{
          1.00E-5, 1.00E-1, 1.09E+6, 1.09E+6, 2.00E+6, 5.00E+6, 5.50E+6,
          6.00E+6, 6.50E+6, 7.00E+6, 7.50E+6, 8.00E+6, 1.42E+7, 2.00E+7
        };
        std::vector< double > refB{ 0.0, 0.0, 0.0, 1.3, 2.3, 3.3, 4.3, 5.3,
                                    6.3, 7.3, 8.3, 9.3, 10.3, 11.3 };

        auto production = 
          reaction.productions< std::vector< interp::Variant > >()[ 0 ];

        auto xs = std::get< njoy::RECONR::interp::LinearLinear >( 
          production[ 0 ] );
        auto energies = xs.x() | ranges::to_vector;
        auto barns = xs.y() | ranges::to_vector;

        details::checkRanges( refE, energies );
        details::checkRanges( refB, barns );
      } // THEN
      THEN( "MT=18 can be checked" ){

        ReactionID reactionID{ PID{ "n" }, PID{ "h1" }, ReactionType{ 18 } };
        auto reaction = reactions.at( reactionID );

        CHECK( 92238 == reaction.ZA() );
        CHECK( 245.0248 == Approx( reaction.atomicWeightRatio() ) );

        std::vector< double > refE{ 1.0E-5, 3.0E7 };
        std::vector< double > refB{ 10.18, 11.18 };

        auto production = 
          reaction.productions< std::vector< interp::Variant > >()[ 0 ];

        auto xs = std::get< njoy::RECONR::interp::LinearLinear >( 
          production[ 0 ] );
        auto energies = xs.x() | ranges::to_vector;
        auto barns = xs.y() | ranges::to_vector;

        details::checkRanges( refE, energies );
        details::checkRanges( refB, barns );
      } // THEN
    } // WHEN
    
  } // GIVEN
  
} // SCENARIO
