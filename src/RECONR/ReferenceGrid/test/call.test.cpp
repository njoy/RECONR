#include <iostream>

#include "catch.hpp"
#include "RECONR.hpp"

namespace RP = njoy::ENDFtk::resonanceParameters;

RP::resolved::SLBW breitWigner();
RP::resolved::ReichMoore reichMoore();
RP::SpecialCase specialCase();

template< typename T >
void ignore( T&& ){}

SCENARIO( "Extracting the reference grid" ){
  using namespace njoy::RECONR;

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
    REQUIRE( reference == trial );
  }

  GIVEN( "a Reich-Moore resolved region" ){
    RP::resolved::ReichMoore rm = reichMoore();

    const std::vector< double > resonanceEnergies =
      { -1.000000E+2, -9.000000E+1, -4.297600E+0, -3.493400E+0,
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
          REQUIRE( reference == trial );
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
          REQUIRE( reference == Approx(trial) );
        }
      }
    }

    THEN( "the reference grid..." ){
      const auto grid = referenceGrid( rm );

      SECTION("will be sorted"){
        REQUIRE( ranges::is_sorted( grid ) );
      }

      SECTION("will be unique"){
        REQUIRE( grid.end() == std::adjacent_find( grid.begin(), grid.end() ) );
      }

      SECTION("will be bounded by the range limits"){
        REQUIRE( grid.front() == 1E-5 );
        REQUIRE( grid.back() == Approx(3.2) );
      }
    }
  }

  GIVEN( "A \"special case\" region" ){
    RP::SpecialCase sc = specialCase();

    THEN( "the boundaries of the region will be extracted" ){
      std::vector<double> grid = referenceGrid( sc );
      REQUIRE( grid.size() == 2 );
      REQUIRE( grid.front() == 1.0 );
      REQUIRE( grid.back() == Approx(2.0) );
    }
  }
}

std::string breitWignerString();
std::string reichMooreString();
std::string specialCaseString();

RP::resolved::SLBW breitWigner(){
  int MAT = 6922;
  int MF = 2;
  int MT = 151;
  long ln = 0;

  auto bws = breitWignerString();
  auto begin = bws.begin();
  auto end = bws.end();

  RP::Base base( 1E-5, 3.2, 1, 1, 0, 0 );
  return RP::resolved::SLBW( base, begin, end, ln, MAT, MF, MT );
}

RP::resolved::ReichMoore reichMoore(){
  long ln = 0;
  int MAT = 6922;
  int MF = 2;
  int MT = 151;
  auto rms = reichMooreString();
  auto begin = rms.begin();
  auto end = rms.end();

  RP::Base base(1E-5, 3.2, 1, 3, 0, 0);
  return RP::resolved::ReichMoore( base, begin, end, ln, MAT, MF, MT );
}

RP::SpecialCase specialCase(){
  int MAT = 125;
  int MF = 2;
  int MT = 151;
  long ln = 0;

  auto scs = specialCaseString();
  auto begin = scs.begin();
  auto end = scs.end();

  RP::Base base( 1.0, 2.0, 0, 0, 0, 0 );
  return RP::SpecialCase( base, begin, end, ln, MAT, MF, MT );
}

std::string breitWignerString(){
  return
    /* cont record */
  " 3.000000+0 0.000000+0          0          0          1          06922 2151   23\n"
    /* list record */
  " 1.664920+2 0.000000+0          0          0         24          46922 2151   24\n"
  "-2.974700+0 2.500000+0 7.846160-2 4.616000-4 7.800000-2 0.000000+06922 2151   25\n"
  "-9.747000-1 2.500000+0 7.846160-2 4.616000-4 7.800000-2 0.000000+06922 2151   26\n"
  " 1.025300+0 2.500000+0 7.846160-2 4.616000-4 7.800000-2 0.000000+06922 2151   27\n"
  " 3.025300+0 2.500000+0 7.846160-2 4.616000-4 7.800000-2 0.000000+06922 2151   28\n"
  " 0.000000+0 0.000000+0          0          0          0          06922 2  099999\n";
}

std::string reichMooreString(){
  return
    " 3.500000+0 9.859600-1          1          0          1          36922 2151     \n"
    " 2.330200+2 9.859600-1          0          0        102         236922 2151     \n"
    "-1.000000+2 3.000000+0 1.145800-2 3.871290-2 1.229980-4 7.233640-26922 2151     \n"
    "-9.000000+1 4.000000+0 2.422100-6 3.680760-2 5.617020-2-2.168940-16922 2151     \n"
    "-4.297600+0 4.000000+0 7.164100-3 3.481860-2 3.192990-1-1.153500-16922 2151     \n"
    "-3.493400+0 3.000000+0 8.471500-8 3.780160-2-6.760010-3 1.298560-26922 2151     \n"
    "-1.504300+0 3.000000+0 8.519600-8 3.767610-2-7.010690-3 1.232090-26922 2151     \n"
    "-4.116100-1 3.000000+0 1.487500-4 2.984470-2-1.027260-3-1.554150-16922 2151     \n"
    "-1.942800-1 4.000000+0 5.044600-7 3.504170-2 1.989540-1-1.694210-36922 2151     \n"
    " 3.657500-5 4.000000+0 6.50520-11 2.984470-2-5.263430-4 9.645560-46922 2151     \n"
    " 2.819000-1 3.000000+0 4.439200-6 3.837130-2 1.065400-1-4.849860-36922 2151     \n"
    " 1.138900+0 4.000000+0 1.384200-5 4.069500-2-4.640000-6 1.093200-16922 2151     \n"
    " 2.036100+0 3.000000+0 9.358700-6 3.933000-2-7.736600-3-1.573500-36922 2151     \n"
    " 2.776700+0 4.000000+0 1.277200-6 3.887600-2 6.049200-2-4.250300-26922 2151     \n"
    " 3.156600+0 3.000000+0 2.432600-5 3.989600-2-7.995100-2 1.716100-26922 2151     \n"
    " 3.620800+0 4.000000+0 4.184000-5 3.764400-2-2.679600-2 2.849100-26922 2151     \n"
    " 4.850800+0 4.000000+0 7.560500-5 3.801700-2 4.666800-5-3.735100-36922 2151     \n"
    " 5.449700+0 4.000000+0 3.793200-5 3.920100-2-7.874000-2-3.612500-16922 2151     \n"
    " 6.209400+0 3.000000+0 1.654700-4 4.005100-2-1.079400-1 7.385900-26922 2151     \n";
}

std::string specialCaseString(){
  return
    " 5.000000-1 1.276553+0          0          0          0          0 125 2151     \n";
}
