#include "catch.hpp"

#include "RECONR.hpp"

#include "RECONR/details/simpleENDFTestString.hpp"
#include "RECONR/details/interpLambdas.hpp"

template< typename R1, typename R2 >
void checkReferenceRange( const R1&, const R2& );

njoy::RECONR::R2D2::ENDFMaterial_t ENDFMaterial( std::string );

SCENARIO( "Testing the factory of resonance reconstruction data for SLBW" ){
  GIVEN( "an ENDF Material with SLBW parameters" ){
    auto material = ENDFMaterial( "SLBW" );

    WHEN( "the resonance reconstruction data can be extracted" ){
      auto XSs = njoy::RECONR::R2D2::Factory::collectXS( material );

      std::vector< int > MTs{ 1, 16, 18, 102 };
      CHECK( ranges::equal( MTs, ranges::view::keys( XSs ) ) );

      THEN( "MT=1 can be checked" ){
        auto xs = std::get< njoy::RECONR::interp::Histogram >( 
            XSs.at( 1 )[ 0 ] );
        std::vector< double > refE{
          1.0E-5, 2.0E-5, 7.5E+5, 1.9E+7, 1.95E+7, 2.0E+7 };
        std::vector< double > refB{
          1.0, 1.182897E1, 3.347392E-5, 2.751761E-5, 2.731301E-5, 2.710792E-5 };
      
        auto energies = xs.x() | ranges::to_vector;
        auto barns = xs.y() | ranges::to_vector;
      
        checkReferenceRange( refE, energies );
        checkReferenceRange( refB, barns );

        CHECK( 1.0 == Approx( xs( 1.5E-5 ) ) );
        CHECK( 1.0 == Approx( xs( 1.999999999E-5 ) ) );
        CHECK( 1.1828971E1 == Approx( xs( 2.0E-5 ) ) );
        CHECK( 1.1828971E1 == Approx( xs( 1 ) ) );
        CHECK( 3.347392E-5 == Approx( xs( 8E5 ) ) );
        CHECK( 2.751761E-5 == Approx( xs( 1.925E7 ) ) );
        CHECK( 2.731301E-5 == Approx( xs( 1.975E7 ) ) );
      } // THEN
      THEN( "MT=16 can be checked" ){ 
        auto xs = std::get< njoy::RECONR::interp::LogarithmicLogarithmic >( 
            XSs.at( 16 )[ 0 ] );
        std::vector< double > refE{
          1.0E-5, 2.0E-5, 7.5E+5, 1.9E+7, 1.95E+7, 2.0E+7 };
        std::vector< double > refB{
          1.6E1, 1.182897E1, 3.347392E-5, 
          2.751761E-5, 2.731301E-5, 2.710792E-5 };

        auto energies = xs.x() | ranges::to_vector;
        auto barns = xs.y() | ranges::to_vector;

        checkReferenceRange( refE, energies );
        checkReferenceRange( refB, barns );

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
        {
          auto xs1 = std::get< njoy::RECONR::interp::Histogram >(
              XSs.at( 18 )[ 0 ] );
          std::vector< double > refE{ 1.0E+5, 1.5E+5, 7.5E+5 };
          std::vector< double > refB{ 1.8E+1, 1.182897E+1, 2.751761E-5 };

          auto energies = xs1.x() | ranges::to_vector;
          auto barns = xs1.y() | ranges::to_vector;

          checkReferenceRange( refE, energies );
          checkReferenceRange( refB, barns );

          CHECK( 18.0 == Approx( xs1( 1.25E+5 ) ) );
          CHECK( 1.182897E1 == Approx( xs1( 2E+5 ) ) );
          CHECK( 3.347392E-5 == Approx( xs1( 8E5 ) ) );
        }
        {
          // auto xs2 = njoy::RECONR::interp::visitor( XSs.at( 18 )[ 3 ] );
          auto xs2 = std::get< njoy::RECONR::interp::LinearLinear >(
              XSs.at( 18 )[ 1 ] );
          std::vector< double > refE{ 7.5E+5, 1.9E+7, 1.95E+7, 2.0E+7 };
          std::vector< double > refB{ 
            3.347392E-5, 2.751761E-5, 2.731301E-5, 2.719792E-5 };

          auto energies = xs2.x() | ranges::to_vector;
          auto barns = xs2.y() | ranges::to_vector;

          checkReferenceRange( refE, energies );
          checkReferenceRange( refB, barns );

          auto refY = linlinInterpolation( 
            1.925E7, refE[ 0 ], refE[ 1 ], refB[ 0 ], refB[ 1 ] );
          CHECK( refY == Approx( xs2( 1.925E7 ) ) );

          refY = linlinInterpolation( 
            1.975E7, refE[ 1 ], refE[ 2 ], refB[ 1 ], refB[ 2 ] );
          CHECK( refY == Approx( xs2( 1.975E7 ) ) );
        }
      }
      THEN( "MT=102 can be checked" ){ 
        {
          auto xs3 = std::get< njoy::RECONR::interp::LinearLogarithmic >( 
              XSs.at( 102 )[ 0 ] );
          std::vector< double > refE{ 1.0E-5, 2.0E-5, 7.5E+5 };
          std::vector< double > refB{ 1.02E+2, 1.182897E+1, 3.347392E-5 };

          auto energies = xs3.x() | ranges::to_vector;
          auto barns = xs3.y() | ranges::to_vector;

          checkReferenceRange( refE, energies );
          checkReferenceRange( refB, barns );

          double refY = linlogInterpolation( 
            1.5E-5, refE[ 0 ], refE[ 1 ], refB[ 0 ], refB[ 1 ] );
          CHECK( refY == Approx( xs3( 1.5E-5 ) ).margin( 1E-7 ) );

          refY = linlogInterpolation( 
            1, refE[ 1 ], refE[ 2 ], refB[ 1 ], refB[ 2 ] );
          CHECK( refY == Approx( xs3( 1 ) ).margin( 1E-7 ) );

          refY = linlogInterpolation( 
            8E5, refE[ 2 ], refE[ 3 ], refB[ 2 ], refB[ 3 ] );
          CHECK( refY == Approx( xs3( 8E5 ) ).margin( 1E-7 ) );
        }
        {
          auto xs4 = std::get< njoy::RECONR::interp::LogarithmicLinear >( 
              XSs.at( 102 )[ 1 ] );
          std::vector< double > refE{ 7.5E+5, 1.9E+7, 1.95E+7, 2.0E+7 };
          std::vector< double > refB{
             3.347392E-5, 2.751761E-5, 2.731301E-5, 2.710792E-5 };

          auto energies = xs4.x() | ranges::to_vector;
          auto barns = xs4.y() | ranges::to_vector;

          checkReferenceRange( refE, energies );
          checkReferenceRange( refB, barns );

          auto refY = loglinInterpolation( 
            1.925E7, refE[ 0 ], refE[ 1 ], refB[ 0 ], refB[ 1 ] );
          CHECK( refY == Approx( xs4( 1.925E7 ) ).margin( 1E-7 ) );

          refY = loglinInterpolation( 
            1.975E7, refE[ 1 ], refE[ 2 ], refB[ 1 ], refB[ 2 ] );
          CHECK( refY == Approx( xs4( 1.975E7 ) ).margin( 1E-7 ) );
        }
      }
    } // WHEN
  } // GIVEN
} // SCENARIO

template< typename R1, typename R2 >
void checkReferenceRange( const R1& ref, const R2& trial){
  CHECK( ranges::distance( ref ) == ranges::distance( trial ) );
  for( const auto& [r, t ] : ranges::view::zip( ref, trial ) ){
    CHECK( r == Approx( t ) );
  }
}
