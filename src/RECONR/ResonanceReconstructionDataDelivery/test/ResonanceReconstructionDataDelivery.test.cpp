#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "RECONR.hpp"

SCENARIO( "Testing the collection of resonance reconstruction data for SLBW" ){
  GIVEN( "an ENDF Material with SLBW parameters" ){
    auto ENDF = njoy::utility::slurpFileToMemory( "SLBW.endf" );
    auto tape = njoy::ENDFtk::syntaxTree::makeTape( std::move( ENDF ) );
    auto material = tape.materialNumber( 4531 ).front();

    THEN( "the resonance reconstruction data can be extracted" ){
      njoy::RECONR::R2D2 r2d2{ material };

      auto XSs = r2d2.crossSections();

      std::vector< int > MTs{
        1, 2, 4, 16, 17, 22, 28, 51, 52, 53, 54, 55, 56, 57, 91, 
        102, 103, 104, 105, 106, 107 };
      CHECK( ranges::equal( MTs, ranges::view::keys( XSs ) ) );

      auto total = XSs.at( 1 );
      std::vector< double > refEnergies{
        1.000000E-5, 2.530000E-2, 1.000000E+5, 1.000000E+5, 1.310500E+5, 
        1.506300E+5, 2.000000E+5, 2.500000E+5, 3.964800E+5, 4.539100E+5, 
        4.599800E+5, 4.739100E+5, 4.785600E+5, 5.041000E+5, 6.000000E+5, 
        7.000000E+5, 8.000000E+5, 9.000000E+5, 1.000000E+6, 1.145600E+6, 
        1.200000E+6, 1.400000E+6, 1.600000E+6, 1.800000E+6, 2.000000E+6, 
        2.200000E+6, 2.400000E+6, 2.600000E+6, 2.800000E+6, 3.000000E+6, 
        3.200000E+6, 3.400000E+6, 3.600000E+6, 3.800000E+6, 3.974500E+6, 
        4.200000E+6, 4.400000E+6, 4.600000E+6, 4.866600E+6, 5.000000E+6, 
        5.200000E+6, 5.400000E+6, 5.600000E+6, 5.800000E+6, 6.000000E+6, 
        6.250000E+6, 6.500000E+6, 6.750000E+6, 7.000000E+6, 7.112500E+6, 
        7.250000E+6, 7.538900E+6, 7.750000E+6, 8.000000E+6, 8.250000E+6, 
        8.500000E+6, 8.750000E+6, 9.054400E+6, 9.250000E+6, 9.500000E+6, 
        9.750000E+6, 1.000000E+7, 1.020500E+7, 1.050000E+7, 1.075000E+7, 
        1.100000E+7, 1.125000E+7, 1.150000E+7, 1.175000E+7, 1.197900E+7, 
        1.225000E+7, 1.250000E+7, 1.275000E+7, 1.300000E+7, 1.325000E+7, 
        1.350000E+7, 1.375000E+7, 1.400000E+7, 1.425000E+7, 1.450000E+7, 
        1.475000E+7, 1.500000E+7, 1.525000E+7, 1.550000E+7, 1.575000E+7, 
        1.600000E+7, 1.612100E+7, 1.625000E+7, 1.650000E+7, 1.675000E+7, 
        1.700000E+7, 1.725000E+7, 1.750000E+7, 1.775000E+7, 1.800000E+7, 
        1.825000E+7, 1.850000E+7, 1.875000E+7, 1.900000E+7, 1.925000E+7, 
        1.950000E+7, 1.975000E+7, 2.000000E+7 };

      std::vector< double > refBarns{
        0.000000E+0, 0.000000E+0, 0.000000E+0, 7.754100E+0, 7.785300E+0, 
        7.806400E+0, 7.882500E+0, 7.904200E+0, 7.792100E+0, 7.701500E+0, 
        7.690800E+0, 7.665200E+0, 7.656300E+0, 7.605700E+0, 7.400700E+0, 
        7.194700E+0, 6.998700E+0, 6.812500E+0, 6.635300E+0, 6.392400E+0, 
        6.305800E+0, 6.004700E+0, 5.727900E+0, 5.472400E+0, 5.236700E+0, 
        5.020200E+0, 4.823600E+0, 4.647500E+0, 4.492600E+0, 4.359300E+0, 
        4.247500E+0, 4.156700E+0, 4.085600E+0, 4.032900E+0, 4.000500E+0, 
        3.975000E+0, 3.965600E+0, 3.966400E+0, 3.979600E+0, 3.990300E+0, 
        4.009800E+0, 4.032600E+0, 4.057300E+0, 4.083300E+0, 4.109900E+0, 
        4.143300E+0, 4.176400E+0, 4.208800E+0, 4.240300E+0, 4.254100E+0, 
        4.270600E+0, 4.304000E+0, 4.327100E+0, 4.352700E+0, 4.376400E+0, 
        4.397800E+0, 4.416900E+0, 4.436900E+0, 4.447700E+0, 4.459500E+0, 
        4.468800E+0, 4.475800E+0, 4.479900E+0, 4.483200E+0, 4.483900E+0, 
        4.482800E+0, 4.479900E+0, 4.475400E+0, 4.469500E+0, 4.462700E+0, 
        4.453300E+0, 4.443400E+0, 4.432300E+0, 4.420100E+0, 4.406800E+0, 
        4.392600E+0, 4.377300E+0, 4.361200E+0, 4.344200E+0, 4.326300E+0, 
        4.307500E+0, 4.288000E+0, 4.267800E+0, 4.246800E+0, 4.225100E+0, 
        4.202900E+0, 4.191900E+0, 4.180000E+0, 4.156700E+0, 4.133000E+0, 
        4.109000E+0, 4.084700E+0, 4.060200E+0, 4.035600E+0, 4.010900E+0, 
        3.986300E+0, 3.961800E+0, 3.937500E+0, 3.913500E+0, 3.889700E+0, 
        3.866200E+0, 3.843100E+0, 3.820500E+0 };
      njoy::Log::info( "XSs[ 1 ].x" );
      auto energies = total.x();
      auto barns = total.y();
      CHECK( ranges::equal( refEnergies, energies ) );
      CHECK( ranges::equal( refBarns, barns ) );

      CHECK( refEnergies[ 0 ] == energies[ 0 ] );
    } // THEN
  } // GIVEN
} // SCENARIO

SCENARIO( "Testing the collection of resonance reconstruction data for MLBW" ){
  GIVEN( "an ENDF Material with MLBW parameters" ){
    auto ENDF = njoy::utility::slurpFileToMemory( "MLBW.endf" );
    auto tape = njoy::ENDFtk::syntaxTree::makeTape( std::move( ENDF ) );
    auto material = tape.materialNumber( 9349 ).front();

    THEN( "the resonance reconstruction data can be extracted" ){
      njoy::RECONR::R2D2 r2d2{ material };
    } // THEN
  } // GIVEN
} // SCENARIO

SCENARIO( "Testing the collection of resonance reconstruction data for RM" ){
  GIVEN( "an ENDF Material with RM parameters" ){
    auto ENDF = njoy::utility::slurpFileToMemory( "RM.endf" );
    auto tape = njoy::ENDFtk::syntaxTree::makeTape( std::move( ENDF ) );
    auto material = tape.materialNumber( 2631 ).front();

    THEN( "the resonance reconstruction data can be extracted" ){
      njoy::RECONR::R2D2 r2d2{ material };
    } // THEN
  } // GIVEN
} // SCENARIO
