#include "catch.hpp"

#include "RECONR.hpp"

#include "RECONR/details/simpleENDFTestString.hpp"
#include "RECONR/details/interpLambdas.hpp"


njoy::RECONR::R2D2::ENDFMaterial_t ENDFMaterial( std::string );

SCENARIO( "Testing the factory of resonance reconstruction data for SLBW" ){
  GIVEN( "an ENDF Material with SLBW parameters" ){
    auto material = ENDFMaterial( "SLBW" );

    THEN( "the resonance reconstruction data can be extracted" ){
      auto r2d2 = njoy::RECONR::R2D2::Factory( std::move( material ) )();

      auto XSs = r2d2.crossSections();

      std::vector< int > MTs{ 1, 16, 18, 102 };
      CHECK( ranges::equal( MTs, ranges::view::keys( XSs ) ) );
      { auto xs = XSs.at( 1 );
        std::vector< double > refEnergies{
          1.0E-5, 2.0E-5, 7.5E+5, 1.9E+7, 1.95E+7, 2.0E+7 };
        std::vector< double > refBarns{
          1.0, 1.182897E1, 3.347392E-5, 2.751761E-5, 2.731301E-5, 2.710792E-5 };

        auto energies = xs.x() | ranges::to_vector;
        auto barns = xs.y() | ranges::to_vector;

        double x1 = refEnergies[0];
        double x2 = refEnergies[1];
        double y1 = refBarns[0];
        double y2 = refBarns[1];
        double x{ 1.0 };
        auto refY = linlinInterpolation( x, x1, x2, y1, y2 );
        auto y = xs( x );
                                      
        CHECK( refY == y );
        
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
        for(size_t i=0; i< refEnergies.size()-1; i++){
          auto x1 = refEnergies[ i ];
          auto x2 = refEnergies[ i+1 ];
          auto y1 = refBarns[ i ];
          auto y2 = refBarns[ i+1 ];
          auto x = 0.5*( x1 + x2 );
          auto y = linlinInterpolation( x, x1, x2, y1, y2 );
                                        
          CHECK( y == xs( x ) );
        }
      }
      { auto xs = XSs.at( 16 );
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
      }
      { auto xs = XSs.at( 18 );
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
      }
      { auto xs = XSs.at( 102 );
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
      }
    } // THEN
  } // GIVEN
} // SCENARIO

