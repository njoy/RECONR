#include "catch.hpp"

#include "RECONR.hpp"

#include "RECONR/details/simpleENDFTestString.hpp"
#include "RECONR/details/interpLambdas.hpp"


njoy::RECONR::R2D2::ENDFMaterial_t ENDFMaterial( std::string );

SCENARIO( "Testing the factory of resonance reconstruction data for SLBW" ){
  GIVEN( "an ENDF Material with SLBW parameters" ){
    auto material = ENDFMaterial( "SLBW" );

    WHEN( "the resonance reconstruction data can be extracted" ){
      // This calls collectXS
      // auto r2d2 = njoy::RECONR::R2D2::Factory( std::move( material ) )();
      auto XSs = njoy::RECONR::R2D2::Factory::collectXS( material );
      // auto XSs = r2d2.crossSections();

      std::vector< int > MTs{ 1, 16, 18, 102 };
      CHECK( ranges::equal( MTs, ranges::view::keys( XSs ) ) );

      THEN( "MT=1 can be checked" ){
        auto xs = XSs.at( 1 );
        std::vector< double > refEnergies{
          1.0E-5, 2.0E-5, 7.5E+5, 1.9E+7, 1.95E+7, 2.0E+7 };
        std::vector< double > refBarns{
          1.0, 1.182897E1, 3.347392E-5, 2.751761E-5, 2.731301E-5, 2.710792E-5 };
      
        auto energies = xs.x() | ranges::to_vector;
        auto barns = xs.y() | ranges::to_vector;
      
        CHECK( ranges::distance( refEnergies ) == ranges::distance( energies ) );
        CHECK( ranges::distance( refBarns ) == ranges::distance( barns ) );
        for( const auto& [ref, ene ] : 
              ranges::view::zip( refEnergies, energies ) ){
          CHECK( ref == Approx( ene ) );
        }
        for( const auto& [ ref, barn ] : 
              ranges::view::zip( refBarns, barns ) ){
          CHECK( ref == Approx( barn ) );
        }
        CHECK( 1.0 == Approx( xs( 1.5E-5 ) ) );
        CHECK( 1.1828971E1 == Approx( xs( 1 ) ) );
        CHECK( 3.347392E-5 == Approx( xs( 8E5 ) ) );
        CHECK( 2.751761E-5 == Approx( xs( 1.925E7 ) ) );
        CHECK( 2.731301E-5 == Approx( xs( 1.975E7 ) ) );
      } // THEN
      THEN( "MT=16 can be checked" ){ 
        auto xs = XSs.at( 16 );
        std::vector< double > refEnergies{
          1.0E-5, 2.0E-5, 7.5E+5, 1.9E+7, 1.95E+7, 2.0E+7 };
        std::vector< double > refBarns{
          1.6E1, 1.182897E1, 3.347392E-5, 
          2.751761E-5, 2.731301E-5, 2.710792E-5 };

        auto energies = xs.x() | ranges::to_vector;
        auto barns = xs.y() | ranges::to_vector;

        CHECK( ranges::distance( refEnergies ) == ranges::distance( energies ) );
        CHECK( ranges::distance( refBarns ) == ranges::distance( barns ) );
        for( const auto& [ref, ene ] : 
             ranges::view::zip( refEnergies, energies ) ){
          CHECK( ref == Approx( ene ) );
        }
        for( const auto& [ ref, barn ] : 
             ranges::view::zip( refBarns, barns ) ){
          CHECK( ref == Approx( barn ) );
        }

        double refY = loglogInterpolation( 
          1.5E-5, refEnergies[ 0 ], refEnergies[ 1 ],
                  refBarns[ 0 ], refBarns[ 1 ] );
        CHECK( refY == Approx( xs( 1.5E-5 ) ) );
        refY = loglogInterpolation( 
          1, refEnergies[ 1 ], refEnergies[ 2 ],
                  refBarns[ 1 ], refBarns[ 2 ] );
        CHECK( refY == Approx( xs( 1 ) ) );
        refY = loglogInterpolation( 
          8E5, refEnergies[ 2 ], refEnergies[ 3 ],
                  refBarns[ 2 ], refBarns[ 3 ] );
        CHECK( refY == Approx( xs( 8E5 ) ) );
        refY = loglogInterpolation( 
          1.925E7, refEnergies[ 3 ], refEnergies[ 4 ],
                   refBarns[ 3 ], refBarns[ 4 ] );
        CHECK( refY == Approx( xs( 1.925E7 ) ) );
        refY = loglogInterpolation( 
          1.975E7, refEnergies[ 4 ], refEnergies[ 5 ],
                  refBarns[ 4 ], refBarns[ 5 ] );
        CHECK( refY == Approx( xs( 1.975E7 ) ) );
      }
      THEN( "MT=18 can be checked" ){ 
        auto xs = XSs.at( 18 );
        std::vector< double > refEnergies{
          1.0E+5, 1.5E+5, 7.5E+5, 1.9E+7, 1.95E+7, 2.0E+7 };
        std::vector< double > refBarns{
          1.8E+1, 1.182897E+1, 3.347392E-5, 
          2.751761E-5, 2.731301E-5, 2.710792E-5 };

        auto energies = xs.x() | ranges::to_vector;
        auto barns = xs.y() | ranges::to_vector;

        CHECK( ranges::distance( refEnergies ) == ranges::distance( energies ) );
        CHECK( ranges::distance( refBarns ) == ranges::distance( barns ) );
        for( const auto& [ref, ene ] : 
             ranges::view::zip( refEnergies, energies ) ){
          CHECK( ref == Approx( ene ) );
        }
        for( const auto& [ ref, barn ] : 
             ranges::view::zip( refBarns, barns ) ){
          CHECK( ref == Approx( barn ) );
        }
        double refY = histoInterpolation( 
          1.5E-5, refEnergies[ 0 ], refEnergies[ 1 ],
                  refBarns[ 0 ], refBarns[ 1 ] );
        CHECK( refY == Approx( xs( 1.5E-5 ) ) );
        refY = histoInterpolation( 
          1, refEnergies[ 1 ], refEnergies[ 2 ],
                  refBarns[ 1 ], refBarns[ 2 ] );
        CHECK( refY == Approx( xs( 1 ) ) );
        refY = histoInterpolation( 
          8E5, refEnergies[ 2 ], refEnergies[ 3 ],
                  refBarns[ 2 ], refBarns[ 3 ] );
        CHECK( refY == Approx( xs( 8E5 ) ) );
        refY = linlinInterpolation( 
          1.925E7, refEnergies[ 3 ], refEnergies[ 4 ],
                   refBarns[ 3 ], refBarns[ 4 ] );
        CHECK( refY == Approx( xs( 1.925E7 ) ) );
        refY = linlinInterpolation( 
          1.975E7, refEnergies[ 4 ], refEnergies[ 5 ],
                  refBarns[ 4 ], refBarns[ 5 ] );
        CHECK( refY == Approx( xs( 1.975E7 ) ) );
      }
      THEN( "MT=102 can be checked" ){ 
        auto xs = XSs.at( 102 );
        std::vector< double > refEnergies{
          1.0E-5, 2.0E-5, 7.5E+5, 1.9E+7, 1.95E+7, 2.0E+7 };
        std::vector< double > refBarns{
          1.02E+2, 1.182897E+1, 3.347392E-5, 
          2.751761E-5, 2.731301E-5, 2.710792E-5 };

        auto energies = xs.x() | ranges::to_vector;
        auto barns = xs.y() | ranges::to_vector;

        CHECK( ranges::distance( refEnergies ) == ranges::distance( energies ) );
        CHECK( ranges::distance( refBarns ) == ranges::distance( barns ) );
        for( const auto& [ref, ene ] : 
             ranges::view::zip( refEnergies, energies ) ){
          CHECK( ref == Approx( ene ) );
        }
        for( const auto& [ ref, barn ] : 
             ranges::view::zip( refBarns, barns ) ){
          CHECK( ref == Approx( barn ) );
        }
        double refY = linlogInterpolation( 
          1.5E-5, refEnergies[ 0 ], refEnergies[ 1 ],
                  refBarns[ 0 ], refBarns[ 1 ] );
        CHECK( refY == Approx( xs( 1.5E-5 ) ).margin( 1E-7 ) );
        refY = linlogInterpolation( 
          1, refEnergies[ 1 ], refEnergies[ 2 ],
                  refBarns[ 1 ], refBarns[ 2 ] );
        CHECK( refY == Approx( xs( 1 ) ).margin( 1E-7 ) );
        refY = linlogInterpolation( 
          8E5, refEnergies[ 2 ], refEnergies[ 3 ],
                  refBarns[ 2 ], refBarns[ 3 ] );
        CHECK( refY == Approx( xs( 8E5 ) ).margin( 1E-7 ) );
        refY = loglinInterpolation( 
          1.925E7, refEnergies[ 3 ], refEnergies[ 4 ],
                   refBarns[ 3 ], refBarns[ 4 ] );
        CHECK( refY == Approx( xs( 1.925E7 ) ).margin( 1E-7 ) );
        refY = loglinInterpolation( 
          1.975E7, refEnergies[ 4 ], refEnergies[ 5 ],
                  refBarns[ 4 ], refBarns[ 5 ] );
        CHECK( refY == Approx( xs( 1.975E7 ) ).margin( 1E-7 ) );
      }
    } // WHEN
  } // GIVEN
} // SCENARIO

