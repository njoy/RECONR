#include "catch.hpp"

#include "RECONR.hpp"

njoy::ENDFtk::syntaxTree::Material< std::string::iterator > ENDFMaterial( std::string );

SCENARIO( "Testing the collection of resonance reconstruction data for SLBW" ){
  GIVEN( "an ENDF Material with SLBW parameters" ){
    auto material = ENDFMaterial( "SLBW" );

    THEN( "the resonance reconstruction data can be extracted" ){
      njoy::RECONR::R2D2 r2d2{ std::move( material ) };

      auto XSs = r2d2.crossSections();

      std::vector< int > MTs{ 1, 16, 18, 102 };
      CHECK( ranges::equal( MTs, ranges::view::keys( XSs ) ) );
      {
        auto xs = XSs.at( 18 );
        std::vector< double > refEnergies{
          1.0E+5, 1.5E+5, 7.5E+5, 1.9E+7, 1.95E+7, 2.0E+7 };
        std::vector< double > refBarns{
          0.000000E+0, 1.182897E+1, 3.347392E-5, 
          2.751761E-5, 2.731301E-5, 2.710792E-5 };

        auto energies = xs.x() | ranges::to_vector;
        auto barns = xs.y() | ranges::to_vector;

        CHECK( ranges::distance( refEnergies ) == ranges::distance( energies ) );
        CHECK( refEnergies == energies );
        CHECK( ranges::distance( refBarns ) == ranges::distance( barns ) );
        for( const auto& p : ranges::view::zip( refEnergies, energies ) ){
          CHECK( std::get< 0 >( p ) == Approx( std::get< 1 >( p ) ) );
        }
      }
    } // THEN
  } // GIVEN
} // SCENARIO

