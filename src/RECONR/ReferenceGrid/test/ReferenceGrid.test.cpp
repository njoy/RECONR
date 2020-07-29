#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "RECONR.hpp"

#include "RECONR/details/simpleENDFTestString.hpp"

namespace RP = njoy::ENDFtk::resonanceParameters;

class TestReferenceGrid: public njoy::RECONR::ReferenceGrid{
public:
  using ReferenceGrid::fill;
};


RP::Isotope isotope();
RP::resolved::SLBW breitWigner();
RP::resolved::ReichMoore reichMoore();
RP::resolved::RMatrixLimited rMatrixLimited();
RP::SpecialCase specialCase();
RP::unresolved::EnergyIndependent caseA();
RP::unresolved::EnergyDependentFissionWidths caseB();
RP::unresolved::EnergyDependent caseC();

static double nextafter( const double value ){
  return std::nextafter( value, std::numeric_limits< double >::infinity() );
}

SCENARIO( "Extracting the reference grid" ){
  using namespace njoy::RECONR;

  GIVEN( "an Isotope" ){

    const std::vector< double > resonanceEnergies { 
      0.00001, 0.9860692, 1.0253, 1.0645308, 2.9860692, 3.0253, 3.0645308, 7.5,
      nextafter( 7.5 )
    };
    RP::Isotope iso = isotope();

    auto trial = referenceGrid( iso );

    CHECK( ranges::distance( resonanceEnergies ) == ranges::distance( trial ) );
    for(const auto& [ref, tri]: ranges::view::zip( resonanceEnergies, trial ) ){
      CHECK( ref == Approx( tri ) );
    }
    
  } // GIVEN

  GIVEN( "Breit-Wigner resonance" ){
    RP::resolved::SLBW slbw = breitWigner();
    const auto resonance = slbw.lValues().front().resonances().front();

    const double resonanceEnergy = resonance.ER();
    const double resonanceWidth = resonance.GT();

    const std::array< double, 3 > reference =
      {{ resonanceEnergy - 0.5 * resonanceWidth,
         resonanceEnergy,
         resonanceEnergy + 0.5 * resonanceWidth }};

    const auto trial = referenceGrid(resonance);
    CHECK( reference == trial );
  }

  GIVEN( "a Reich-Moore resolved region" ){
    RP::resolved::ReichMoore rm = reichMoore();

    const std::vector< double > resonanceEnergies { 
      -1.000000E+2, -9.000000E+1, -4.297600E+0, -3.493400E+0,
      -1.504300E+0, -4.116100E-1, -1.942800E-1,  3.657500E-5,
       2.819000E-1,  1.138900E+0,  2.036100E+0,  2.776700E+0,
       3.156600E+0,  3.620800E+0,  4.850800E+0,  5.449700E+0 };

    GIVEN( "an l-value" ){
      const auto lValue = rm.lValues().front();

      GIVEN( "a resonance" ){
        const auto resonance = lValue.resonances().front();

        THEN("the reference grid will be composed of 3-arrays"){
          const double resonanceEnergy = resonanceEnergies.front();
          const double resonanceWidth = resonance.GN()
                                        + resonance.GG()
                                        + resonance.GFA()
                                        + resonance.GFB();

          const std::array< double, 3 > reference =
            {{ resonanceEnergy - 0.5 * resonanceWidth,    // left full-width half-max
               resonanceEnergy,                           // resonance peak
               resonanceEnergy + 0.5 * resonanceWidth }}; // right full-width half-max

          const auto trial = referenceGrid(resonance);
          CHECK( reference == trial );
        }
      }

      THEN( "the reference grid will have an entry for each resonance" ){
        const auto& reference = resonanceEnergies;

        const auto resonanceArrays = referenceGrid( lValue );
        const auto trial =
          resonanceArrays
          | ranges::view::transform
            ( []( const auto& resonanceArray )
              { return resonanceArray[1]; } );

        RANGES_FOR( auto pair, ranges::view::zip( reference, trial ) ){
          auto reference = std::get<0>(pair);
          auto trial = std::get<1>(pair);
          CHECK( reference == Approx(trial) );
        }
      }
    }

    THEN( "the reference grid..." ){
      double lowerEnergy{ 9.8596E-1 };
      double upperEnergy{ 5.5 };
      const auto grid = referenceGrid( rm, lowerEnergy, upperEnergy );

      SECTION("will be sorted"){
        CHECK( ranges::is_sorted( grid ) );
      }

      SECTION("will be unique"){
        CHECK( grid.end() == std::adjacent_find( grid.begin(), grid.end() ) );
      }

      SECTION("will be bounded by the range limits"){
        CHECK( grid.front() == Approx( lowerEnergy ) );
        CHECK( grid.back() == Approx( upperEnergy ) );
      }
    }
  }

  GIVEN( "a RMatrixLimited resolved region" ){
    RP::resolved::RMatrixLimited rml = rMatrixLimited();
    
    std::vector< double > reference{
      -1.223300e+6, 7.788000e+3, 5.152000e+4, 5.359000e+4 };

    double lowerEnergy{ -1.9E6 };
    double upperEnergy{ 5.5E5 };

    reference.insert( reference.cbegin(), lowerEnergy );
    reference.insert( reference.cend(), upperEnergy );
    reference.insert( reference.cend(), nextafter( upperEnergy ) );

    const auto grid = referenceGrid( rml, lowerEnergy, upperEnergy );

    CHECK( reference == grid );
  } // GIVEN

  GIVEN( "a \"special case\" region" ){
    RP::SpecialCase sc = specialCase();

    THEN( "the boundaries of the region will be extracted" ){
      std::vector<double> grid = referenceGrid( sc, 0.5, 1.276553 );
      CHECK( grid.size() == 3 );
      CHECK( grid[ 0 ] == 0.5 );
      CHECK( grid[ 1 ] == Approx( 1.276553 ) );
      CHECK( grid[ 2 ] == Approx( 1.276553 ) );
    }
  }

  GIVEN( "a Case A unresolved region" ){
    RP::unresolved::EnergyIndependent ca = caseA();

    std::vector<double> refEnergies{
      23.0000000000E3, 27.4568627590E3, 32.7773614170E3, 39.1288484370E3,
      46.7111052810E3, 55.7626263930E3, 66.5681208680E3, 79.4674677760E3,
      94.8664068070E3, 100E3 
    };
    auto energies = referenceGrid( ca, 2.3E4, 1E5 );

    // njoy::Log::info( "refEnergies: {}", refEnergies | ranges::view::all );
    // njoy::Log::info( "   Energies: {}", energies | ranges::view::all );
    RANGES_FOR( auto en, ranges::view::zip( refEnergies, energies ) ){
      auto [ ref, trial ] = en;
      CHECK( ref == Approx( trial ) );
    }
  }

  GIVEN( "a Case B unresolved region" ){
    RP::unresolved::EnergyDependentFissionWidths cb = caseB();

    std::vector<double> refEnergies{
      5.7E3, 6.80453E3, 7.0E3, 8.0E3, 9.0E3, 
      1.0E4, 1.19377664E4, 1.2E4,
      1.4E4, 1.6E4, 1.8E4, 
      2.0E4, 2.38755328E4, 2.5E4, 2.9844416E4,
      3.0E4, 3.5E4, 4.0E4
    };
    auto energies = referenceGrid( cb, 5.7E3, 4.0E4 );

    RANGES_FOR( auto en, ranges::view::zip( refEnergies, energies ) ){
      auto [ ref, trial ] = en;
      CHECK( ref == Approx( trial ) );
    }
  }

  GIVEN( "a Case C unresolved region" ){
    RP::unresolved::EnergyDependent cc = caseC();

    std::vector<double> refEnergies{
      6.0E+3,
      7.0E+3,
      8.0E+3, 9.55021313E3,
      1.0E+4, 1.19377664E4, 1.42510267E4,
      1.5E+4, 1.79066496E4, 2.13765400E4,
      2.5E+4, 2.98444160E4,
      3.0E+4, 3.58132993E4,
      4.0E+4, 4.77510657E4,
      5.0E+4, 5.96888321E4,
      6.0E+4,
      7.0E+4,
      8.0E+4, 9.550213134E4,
      1.0E+5,
    };
    auto energies = referenceGrid( cc, 6.0E3, 1.0E5 );

    RANGES_FOR( auto en, ranges::view::zip( refEnergies, energies ) ){
      auto ref = std::get< 0 >( en );
      auto trial = std::get< 1 >( en );
      CHECK( ref == Approx( trial ) );
    }
  }
}

SCENARIO( "Filling the sparse energy grid" ){
  using namespace njoy::RECONR;

  GIVEN( "an 'energy grid'" ){
    std::vector< double > grid{ 1.0 };

    WHEN( "filling upper grid is within 1.19... of lower grid" ){
      TestReferenceGrid::fill ( 1.0, 1.1, grid );
      THEN( "only upper bound is added" ){
        std::vector< double > refGrid{1.0, 1.1};
        REQUIRE( refGrid == grid );
      }
    }

    WHEN( "filling is needed" ){
      THEN( "the appropriate values are appended" ){
        TestReferenceGrid::fill( 1.0, 2.0, grid );
        std::vector< double > refGrid = {{ 
          1.0, 1.1937766417, 1.4251026703, 1.7012542799, 2.0 }};
        for(size_t i=0; i<refGrid.size(); i++){
          REQUIRE( refGrid[i] == Approx( grid[i] ) );
        }
      }
      THEN( "the appropriate values are appended" ){
        TestReferenceGrid::fill( 4, 15, grid );
        std::vector< double > refGrid = {{ 
          1.0, 4.7751065669, 5.7004106812, 6.8050171194, 8.1236704836,
          9.6978480683, 11.5770644989, 13.8204291784, 15.0 }};

        for(size_t i=0; i<refGrid.size(); i++){
          REQUIRE( refGrid[i] == Approx( grid[i] ) );
        }
      }
    }

    WHEN( "upperBound < lowerBound" ){
      TestReferenceGrid::fill ( 1.0, 0.9, grid );
      THEN( "nothing is changed" ){
        std::vector< double > refGrid{ 1, 0.9 };
        REQUIRE( refGrid == grid );
      }
    }
  }
} // SCENARIO

std::string specialCaseString(){
  return
    " 5.000000-1 1.276553+0          0          0          0          0 125 2151     \n";
}

std::string caseAString(){
  return
      // base
    // " 2.300000+4 1.000000+5          2          1          0          05655 2151\n"
    // range CONT
    " 0.000000+0 6.233000-1          0          0          3          05655 2151     \n"
    // L=0 LIST                                                                      
    " 1.387090+2 0.000000+0          0          0          6          15655 2151     \n"
    " 4.400000+3 5.000000-1 1.000000+0 4.400000-1 5.000000-2 0.000000+05655 2151     \n"
    // L=1 LIST                                                                      
    " 1.387090+2 0.000000+0          1          0         12          25655 2151     \n"
    " 4.400000+3 5.000000-1 1.000000+0 5.280000-1 9.000000-2 0.000000+05655 2151     \n"
    " 2.200000+3 1.500000+0 1.000000+0 2.640000-1 9.000000-2 0.000000+05655 2151     \n"
    // L=2 LIST                                                                      
    " 1.387090+2 0.000000+0          2          0         12          25655 2151     \n"
    " 2.200000+3 1.500000+0 1.000000+0 3.300000-2 5.000000-2 0.000000+05655 2151     \n"
    " 1.466670+3 2.500000+0 1.000000+0 2.200000-2 5.000000-2 0.000000+05655 2151     \n";
}

std::string caseBString(){
  return 
    // " 5.700000+3 4.000000+4          2          1          0          09440 2151\n"
    // LIST
    " 0.000000+0 8.880000-1          0          0         14          39440 2151     \n"
    " 5.700000+3 7.000000+3 8.000000+3 9.000000+3 1.000000+4 1.200000+49440 2151     \n"
    " 1.400000+4 1.600000+4 1.800000+4 2.000000+4 2.500000+4 3.000000+49440 2151     \n"
    " 3.500000+4 4.000000+4                                            9440 2151     \n"
    // L=0 CONT                                                                      
    " 2.379920+2 0.000000+0          0          0          1          09440 2151     \n"
    // L=0 LIST                                                                      
    " 0.000000+0 0.000000+0          0          1         20          09440 2151     \n"
    " 1.310000+1 5.000000-1 1.000000+0 1.572000-3 3.100000-2 0.000000+09440 2151     \n"
    " 1.256000-3 1.544000-3 1.825000-3 2.025000-3 2.119000-3 2.051000-39440 2151     \n"
    " 1.992000-3 1.879000-3 1.860000-3 1.838000-3 1.694000-3 1.581000-39440 2151     \n"
    " 1.481000-3 1.403000-3                                            9440 2151     \n"
    // L=1 CONT                                                                      
    " 2.379920+2 0.000000+0          1          0          2          09440 2151     \n"
    // L=1,J=0 LIST                                                                  
    " 0.000000+0 0.000000+0          1          1         20          09440 2151     \n"
    " 1.310000+1 5.000000-1 1.000000+0 3.013000-3 3.100000-2 0.000000+09440 2151     \n"
    " 4.314000-3 4.572000-3 4.740000-3 5.000000-3 5.520000-3 7.057000-39440 2151     \n"
    " 8.251000-3 9.276000-3 9.930000-3 1.035000-2 1.210000-2 1.341000-29440 2151     \n"
    " 1.456000-2 1.542000-2                                            9440 2151     \n"
    // L=1,J=1 LIST                                                                  
    " 0.000000+0 0.000000+0          1          2         20          09440 2151     \n"
    " 6.697000+0 1.500000+0 1.000000+0 1.540000-3 3.100000-2 0.000000+09440 2151     \n"
    " 1.207000-3 1.299000-3 1.326000-3 1.397000-3 1.544000-3 1.973000-39440 2151     \n"
    " 2.317000-3 2.581000-3 2.811000-3 3.075000-3 3.494000-3 3.887000-39440 2151     \n"
    " 4.290000-3 4.586000-3                                            9440 2151     \n"
    // L=2 CONT                                                                      
    " 2.379920+2 0.000000+0          2          0          2          09440 2151     \n"
    // L=2,J=0 LIST                                                                  
    " 0.000000+0 0.000000+0          2          1         20          09440 2151     \n"
    " 6.697000+0 1.500000+0 1.000000+0 8.304300-4 3.100000-2 0.000000+09440 2151     \n"
    " 1.000000-9 1.000000-9 1.000000-9 1.000000-9 1.000000-9 1.000000-99440 2151     \n"
    " 1.000000-9 1.000000-9 1.000000-9 1.000000-9 1.000000-9 1.000000-99440 2151     \n"
    " 1.000000-9 1.000000-9                                            9440 2151     \n"
    // L=2,J=1 LIST                                                                  
    " 0.000000+0 0.000000+0          2          2         20          09440 2151     \n"
    " 4.633000+0 2.500000+0 1.000000+0 5.744900-4 3.100000-2 0.000000+09440 2151     \n"
    " 1.000000-9 1.000000-9 1.000000-9 1.000000-9 1.000000-9 1.000000-99440 2151     \n"
    " 1.000000-9 1.000000-9 1.000000-9 1.000000-9 1.000000-9 1.000000-99440 2151     \n"
    " 1.000000-9 1.000000-9                                            9440 2151     \n";
}

std::string caseCString(){
  return
    /* Isotope
       " 3.809000+4 8.913540+1          0          0          1          03843 2151    1\n"
       " 3.809000+4 1.000000+0          0          1          1          03843 2151    2\n"
    */
    /* base
       " 6.000000+3 1.000000+5          2          2          0          03843 2151    3\n"
    */
    // range CONT
    " 0.000000+0 6.795900-1          0          0          3          03843 2151     \n"
    // L=0 CONT                                                                      
    " 8.913540+1 0.000000+0          0          0          1          03843 2151     \n"
    // L=0 LIST                                                                      
    " 5.000000-1 0.000000+0          5          0         84         133843 2151     \n"
    " 0.000000+0 0.000000+0 1.000000+0 2.000000+0 3.000000+0 4.000000+03843 2151     \n"
    " 6.000000+3 1.240600+4 0.000000+0 4.590100-1 2.050000-1 0.000000+03843 2151     \n"
    " 7.000000+3 1.240600+4 0.000000+0 4.590100-1 2.050000-1 0.000000+03843 2151     \n"
    " 8.000000+3 1.240600+4 0.000000+0 4.590100-1 2.050000-1 0.000000+03843 2151     \n"
    " 1.000000+4 1.231300+4 0.000000+0 4.555800-1 2.050000-1 0.000000+03843 2151     \n"
    " 1.500000+4 1.226300+4 0.000000+0 4.537400-1 2.050000-1 0.000000+03843 2151     \n"
    " 2.500000+4 1.212200+4 0.000000+0 4.485000-1 2.050000-1 0.000000+03843 2151     \n"
    " 3.000000+4 1.205400+4 0.000000+0 4.460100-1 2.050000-1 0.000000+03843 2151     \n"
    " 4.000000+4 1.191800+4 0.000000+0 4.409700-1 2.050000-1 0.000000+03843 2151     \n"
    " 5.000000+4 1.181000+4 0.000000+0 4.369800-1 2.050000-1 0.000000+03843 2151     \n"
    " 6.000000+4 1.113000+4 0.000000+0 4.118100-1 2.050000-1 0.000000+03843 2151     \n"
    " 7.000000+4 1.077400+4 0.000000+0 3.986200-1 2.050000-1 0.000000+03843 2151     \n"
    " 8.000000+4 1.039500+4 0.000000+0 3.846100-1 2.050000-1 0.000000+03843 2151     \n"
    " 1.000000+5 9.669700+3 0.000000+0 3.577800-1 2.050000-1 0.000000+03843 2151     \n"
    // L=1 CONT                                                                      
    " 8.913540+1 0.000000+0          1          0          2          03843 2151     \n"
    // L=1,J=0 LIST                                                                  
    " 5.000000-1 0.000000+0          5          0         84         133843 2151     \n"
    " 0.000000+0 0.000000+0 0.000000+0 1.000000+0 0.000000+0 0.000000+03843 2151     \n"
    " 6.000000+3 1.240600+4 0.000000+0 6.723800+0 2.050000-1 0.000000+03843 2151     \n"
    " 7.000000+3 1.240600+4 0.000000+0 6.723800+0 2.050000-1 0.000000+03843 2151     \n"
    " 8.000000+3 1.240600+4 0.000000+0 6.723800+0 2.050000-1 0.000000+03843 2151     \n"
    " 1.000000+4 1.231300+4 0.000000+0 6.673600+0 2.050000-1 0.000000+03843 2151     \n"
    " 1.500000+4 1.226300+4 0.000000+0 6.646600+0 2.050000-1 0.000000+03843 2151     \n"
    " 2.500000+4 1.212200+4 0.000000+0 6.569900+0 2.050000-1 0.000000+03843 2151     \n"
    " 3.000000+4 1.205400+4 0.000000+0 6.533500+0 2.050000-1 0.000000+03843 2151     \n"
    " 4.000000+4 1.191800+4 0.000000+0 6.459600+0 2.050000-1 0.000000+03843 2151     \n"
    " 5.000000+4 1.181000+4 0.000000+0 6.401200+0 2.050000-1 0.000000+03843 2151     \n"
    " 6.000000+4 1.113000+4 0.000000+0 6.032400+0 2.050000-1 0.000000+03843 2151     \n"
    " 7.000000+4 1.077400+4 0.000000+0 5.839300+0 2.050000-1 0.000000+03843 2151     \n"
    " 8.000000+4 1.039500+4 0.000000+0 5.634100+0 2.050000-1 0.000000+03843 2151     \n"
    " 1.000000+5 9.669700+3 0.000000+0 5.241000+0 2.050000-1 0.000000+03843 2151     \n"
    // L=1,J=1 LIST                                                                  
    " 1.500000+0 0.000000+0          5          0         84         133843 2151     \n"
    " 0.000000+0 0.000000+0 0.000000+0 1.000000+0 0.000000+0 0.000000+03843 2151     \n"
    " 6.000000+3 6.202800+3 0.000000+0 3.361900+0 2.050000-1 0.000000+03843 2151     \n"
    " 7.000000+3 6.202800+3 0.000000+0 3.361900+0 2.050000-1 0.000000+03843 2151     \n"
    " 8.000000+3 6.202800+3 0.000000+0 3.361900+0 2.050000-1 0.000000+03843 2151     \n"
    " 1.000000+4 6.156500+3 0.000000+0 3.336800+0 2.050000-1 0.000000+03843 2151     \n"
    " 1.500000+4 6.131600+3 0.000000+0 3.323300+0 2.050000-1 0.000000+03843 2151     \n"
    " 2.500000+4 6.060800+3 0.000000+0 3.285000+0 2.050000-1 0.000000+03843 2151     \n"
    " 3.000000+4 6.027200+3 0.000000+0 3.266800+0 2.050000-1 0.000000+03843 2151     \n"
    " 4.000000+4 5.959100+3 0.000000+0 3.229800+0 2.050000-1 0.000000+03843 2151     \n"
    " 5.000000+4 5.905100+3 0.000000+0 3.200600+0 2.050000-1 0.000000+03843 2151     \n"
    " 6.000000+4 5.564900+3 0.000000+0 3.016200+0 2.050000-1 0.000000+03843 2151     \n"
    " 7.000000+4 5.386800+3 0.000000+0 2.919600+0 2.050000-1 0.000000+03843 2151     \n"
    " 8.000000+4 5.197500+3 0.000000+0 2.817000+0 2.050000-1 0.000000+03843 2151     \n"
    " 1.000000+5 4.834900+3 0.000000+0 2.620500+0 2.050000-1 0.000000+03843 2151     \n"
    // L=2 CONT                                                                      
    " 8.913540+1 0.000000+0          2          0          2          03843 2151     \n"
    // L=2,J=0 LIST                                                                  
    " 1.500000+0 0.000000+0          5          0         84         133843 2151     \n"
    " 0.000000+0 0.000000+0 0.000000+0 1.000000+0 0.000000+0 0.000000+03843 2151     \n"
    " 6.000000+3 6.202800+3 0.000000+0 2.233000-1 2.050000-1 0.000000+03843 2151     \n"
    " 7.000000+3 6.202800+3 0.000000+0 2.233000-1 2.050000-1 0.000000+03843 2151     \n"
    " 8.000000+3 6.202800+3 0.000000+0 2.233000-1 2.050000-1 0.000000+03843 2151     \n"
    " 1.000000+4 6.156500+3 0.000000+0 2.216300-1 2.050000-1 0.000000+03843 2151     \n"
    " 1.500000+4 6.131600+3 0.000000+0 2.207400-1 2.050000-1 0.000000+03843 2151     \n"
    " 2.500000+4 6.060800+3 0.000000+0 2.181900-1 2.050000-1 0.000000+03843 2151     \n"
    " 3.000000+4 6.027200+3 0.000000+0 2.169800-1 2.050000-1 0.000000+03843 2151     \n"
    " 4.000000+4 5.959100+3 0.000000+0 2.145300-1 2.050000-1 0.000000+03843 2151     \n"
    " 5.000000+4 5.905100+3 0.000000+0 2.125900-1 2.050000-1 0.000000+03843 2151     \n"
    " 6.000000+4 5.564900+3 0.000000+0 2.003400-1 2.050000-1 0.000000+03843 2151     \n"
    " 7.000000+4 5.386800+3 0.000000+0 1.939200-1 2.050000-1 0.000000+03843 2151     \n"
    " 8.000000+4 5.197500+3 0.000000+0 1.871100-1 2.050000-1 0.000000+03843 2151     \n"
    " 1.000000+5 4.834900+3 0.000000+0 1.740500-1 2.050000-1 0.000000+03843 2151     \n"
    // L=2,J=1 LIST                                                                  
    " 2.500000+0 0.000000+0          5          0         84         133843 2151     \n"
    " 0.000000+0 0.000000+0 0.000000+0 1.000000+0 0.000000+0 0.000000+03843 2151     \n"
    " 6.000000+3 4.135200+3 0.000000+0 1.488700-1 2.050000-1 0.000000+03843 2151     \n"
    " 7.000000+3 4.135200+3 0.000000+0 1.488700-1 2.050000-1 0.000000+03843 2151     \n"
    " 8.000000+3 4.135200+3 0.000000+0 1.488700-1 2.050000-1 0.000000+03843 2151     \n"
    " 1.000000+4 4.104300+3 0.000000+0 1.477600-1 2.050000-1 0.000000+03843 2151     \n"
    " 1.500000+4 4.087700+3 0.000000+0 1.471600-1 2.050000-1 0.000000+03843 2151     \n"
    " 2.500000+4 4.040500+3 0.000000+0 1.454600-1 2.050000-1 0.000000+03843 2151     \n"
    " 3.000000+4 4.018100+3 0.000000+0 1.446500-1 2.050000-1 0.000000+03843 2151     \n"
    " 4.000000+4 3.972700+3 0.000000+0 1.430200-1 2.050000-1 0.000000+03843 2151     \n"
    " 5.000000+4 3.936800+3 0.000000+0 1.417200-1 2.050000-1 0.000000+03843 2151     \n"
    " 6.000000+4 3.710000+3 0.000000+0 1.335600-1 2.050000-1 0.000000+03843 2151     \n"
    " 7.000000+4 3.591200+3 0.000000+0 1.292800-1 2.050000-1 0.000000+03843 2151     \n"
    " 8.000000+4 3.465000+3 0.000000+0 1.247400-1 2.050000-1 0.000000+03843 2151     \n"
    " 1.000000+5 3.223200+3 0.000000+0 1.160400-1 2.050000-1 0.000000+03843 2151     \n";
}                                                                                  

RP::Isotope isotope(){

  auto material = details::ENDFMaterial( "SLBW" );
  auto MT151 = material.fileNumber( 2 ).sectionNumber( 151 ).parse< 2, 151 >();

  return MT151.isotopes().front();
}

RP::resolved::SLBW breitWigner(){

  auto material = details::ENDFMaterial( "SLBW" );
  auto MT151 = material.fileNumber( 2 ).sectionNumber( 151 ).parse< 2, 151 >();

  return std::get< RP::resolved::SingleLevelBreitWigner >(
      MT151.isotopes().front().resonanceRanges().front().parameters() );
}

RP::resolved::ReichMoore reichMoore(){

  auto material = details::ENDFMaterial( "RM" );
  auto MT151 = material.fileNumber( 2 ).sectionNumber( 151 ).parse< 2, 151 >();

  return std::get< RP::resolved::ReichMoore >(
      MT151.isotopes().front().resonanceRanges().front().parameters() );
}

RP::resolved::RMatrixLimited rMatrixLimited(){

  auto material = details::ENDFMaterial( "RML" );
  auto MT151 = material.fileNumber( 2 ).sectionNumber( 151 ).parse< 2, 151 >();

  return std::get< RP::resolved::RMatrixLimited >(
      MT151.isotopes().front().resonanceRanges().front().parameters() );
}

RP::SpecialCase specialCase(){
  int MAT = 125;
  int MF = 2;
  int MT = 151;
  long ln = 0;

  auto scs = specialCaseString();
  auto begin = scs.begin();
  auto end = scs.end();

  // RP::resolved::BreitWignerReichMooreBase base( 1.0, 2.0, 0, 0, 0, 0 );
  return RP::SpecialCase( begin, end, ln, MAT, MF, MT );
}

RP::unresolved::EnergyIndependent caseA(){
    long lineNumber = 0;
    int MAT = 5655;
    int MF = 2;
    int MT = 151;
    
    std::string ENDF = caseAString();
    auto begin = ENDF.begin();
    auto end = ENDF.end();

    // RP::resolved::BreitWignerReichMooreBase base( 2.3E4, 1.0E5, 2, 1, 0, 0 );
          
    return RP::unresolved::EnergyIndependent( 
        begin, end, lineNumber, MAT, MF, MT );
}

RP::unresolved::EnergyDependentFissionWidths caseB(){
    long lineNumber = 0;

    std::string ENDF = caseBString();
    auto begin = ENDF.begin();
    auto end = ENDF.end();

    // RP::resolved::BreitWignerReichMooreBase base( 5.7E3, 4.0E4, 2, 1, 0, 0 );
    return RP::unresolved::EnergyDependentFissionWidths( 
        begin, end, lineNumber, 9440, 2, 151 );
}

RP::unresolved::EnergyDependent caseC(){
    long lineNumber = 0;

    std::string ENDF = caseCString();
    auto begin = ENDF.begin();
    auto end = ENDF.end();

    // RP::resolved::BreitWignerReichMooreBase base( 6.0E3, 1.0E5, 2, 2, 0, 0 );
    return RP::unresolved::EnergyDependent( 
        begin, end, lineNumber, 3843, 2, 151 );
}

