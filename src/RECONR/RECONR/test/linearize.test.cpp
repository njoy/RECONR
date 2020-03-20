#include "catch.hpp"

#include "RECONR.hpp"

#include "RECONR/details/simpleENDFTestString.hpp"

njoy::RECONR::R2D2::ENDFMaterial_t ENDFMaterial( std::string );

template< typename G >
void printV( std::string name, G&& grid ){
  njoy::Log::info( "{}, {}", name, ranges::distance( grid ) );
  njoy::Log::info( "{}", grid | ranges::view::all );
  // for(auto& E : grid ){
  //   njoy::Log::info( "\t{:.16E}", E );
  // }
}

SCENARIO( "Testing the linearization of collected cross sections" ){
  GIVEN( "an ResonanceReconstructionDataDelivery object" ){
    auto material = ENDFMaterial( "SLBW" );
    auto r2d2 = njoy::RECONR::R2D2::Factory( std::move( material ) )();

    WHEN( "the cross sections are linearized" ){
      njoy::RECONR::RECONR::linearize( r2d2, 1E-6, 1E-3 );

      auto rXSs = r2d2.crossSections();
      auto lXSs = r2d2.linearCrossSections();

      auto keys = ranges::view::keys( rXSs );
      CHECK( ranges::equal( keys, ranges::view::keys( lXSs ) ) );
      
      for( auto MT: keys ){
        njoy::Log::info( "MT: {}", MT );
        auto rxs = rXSs.at( MT );
        auto lxs = lXSs.at( MT );

        auto linX = lxs.x() | ranges::to_vector;
        auto linY = lxs.y() | ranges::to_vector;
        // printV( "linX", linX );
        // printV( "linY", linY );
      }

      auto nextMin = [](double value ){ 
        return std::nextafter( value, std::numeric_limits< double >::min() ); };
      THEN( "we can check MT=1" ){
        auto lxs1 = lXSs.at( 1 );
        std::vector< double > refE{
          1.0E-5, 
          nextMin( 2.0E-5 ), 2.0E-5,
          nextMin( 7.5E+5 ), 7.5E+5,
          nextMin( 1.9E+7 ), 1.9E+7,
          nextMin( 1.95E+7 ),1.95E+7,
          nextMin( 2.0E+7 ), 2.0E+7 };
        std::vector< double > refB{
          1.0         , 1.0         , 
          1.182897E1  , 1.182897E1  , 
          3.347392E-5 , 3.347392E-5 , 
          2.751761E-5 , 2.751761E-5 , 
          2.731301E-5 , 2.731301E-5 , 
          2.710792E-5 };

        auto energies = lxs1.x() | ranges::to_vector;
        auto barns = lxs1.y() | ranges::to_vector;
        CHECK( ranges::distance( refE ) == ranges::distance( energies ) );
        CHECK( ranges::distance( refB ) == ranges::distance( barns ) );
        for( const auto& [r, e ] : ranges::view::zip( refE, energies ) ){
          CHECK( r == Approx( e ).scale( 1E-7 ) );
        }
        for( const auto& [r, b ] : ranges::view::zip( refB, barns ) ){
          CHECK( r == Approx( b ).scale( 1E-7 ) );
        }
      }
      THEN( "we can check MT=18" ){
        auto lxs18 = lXSs.at( 18 );
        std::vector< double > refE{ 
          1.0E+5,
          nextMin( 1.5E+5 ), 1.5E+5,
          nextMin( 7.5E+5 ), 7.5E+5,
          1.9E+7, 1.95E+7, 2.0E+7 };

        std::vector< double > refB{ 
          1.8E+1      , 1.8E+1      , 
          1.182897E+1 , 1.182897E+1 , 
          3.347392E-5, 2.751761E-5, 2.731301E-5, 2.710792E-5 };

        auto energies = lxs18.x() | ranges::to_vector;
        auto barns = lxs18.y() | ranges::to_vector;
        CHECK( ranges::distance( refE ) == ranges::distance( energies ) );
        CHECK( ranges::distance( refB ) == ranges::distance( barns ) );
        for( const auto& [r, e ] : ranges::view::zip( refE, energies ) ){
          CHECK( r == Approx( e ).scale( 1E-7 ) );
        }
        for( const auto& [r, b ] : ranges::view::zip( refB, barns ) ){
          CHECK( r == Approx( b ).scale( 1E-7 ) );
        }
      }
    } // THEN
  } // GIVEN
} // SCENARIO
