#include "catch.hpp"

#include "RECONR.hpp"

njoy::ENDFtk::syntaxTree::Material< std::string::iterator > ENDFMaterial( std::string );

SCENARIO( "Testing the collection of resonance reconstruction data for SLBW" ){
  GIVEN( "an ENDF Material with SLBW parameters" ){
    auto material = ENDFMaterial( "SLBW" );
  auto MF3 = material.fileNumber( 3 ).parse< 3 >();

    THEN( "the resonance reconstruction data can be extracted" ){
      // njoy::RECONR::R2D2 r2d2{ std::move( material ) };

      // auto XSs = r2d2.crossSections();

      // std::vector< int > MTs{ 1, 16, 18, 102 };
      // CHECK( ranges::equal( MTs, ranges::view::keys( XSs ) ) );

      // {
      //   auto xs = XSs.at( 18 );
      //   std::vector< double > refEnergies{
      //     1.612100E+7, 1.625000E+7, 1.650000E+7, 1.675000E+7, 1.700000E+7,
      //     1.725000E+7, 1.750000E+7, 1.775000E+7, 1.800000E+7, 1.825000E+7,
      //     1.850000E+7, 1.875000E+7, 1.900000E+7, 1.925000E+7, 1.950000E+7,
      //     1.975000E+7, 2.000000E+7 };

      //   std::vector< double > refBarns{
      //     0.000000E+0, 1.324500E-6, 4.676300E-4, 4.368300E-3, 1.491100E-2,
      //     3.358100E-2, 6.088400E-2, 9.726400E-2, 1.431000E-1, 1.961800E-1,
      //     2.561200E-1, 3.205800E-1, 3.884700E-1, 4.580700E-1, 5.260800E-1,
      //     5.942400E-1, 6.598100E-1 };
      //   auto energies = xs.x() | ranges::to_vector;
      //   auto barns = xs.y() | ranges::to_vector;

      //   CHECK( ranges::distance( refEnergies ) == ranges::distance( energies ) );
      //   CHECK( refEnergies == energies );
      //   CHECK( refBarns == barns );
      // }
    } // THEN
  } // GIVEN
} // SCENARIO

