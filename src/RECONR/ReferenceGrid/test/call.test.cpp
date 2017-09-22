#include <iostream>

#include "catch.hpp"

#include "RECONR.hpp"

namespace RP = njoy::ENDFtk::resonanceParameters;
namespace RECONR = njoy::njoy21::RECONR;

RP::resolved::SLBW sl_bw();
RP::resolved::MLBW ml_bw();
void checkBWResonanceEnergies( const RP::resolved::BreitWigner& BW );

std::string ReichMoore();

double RMHalfwidth( double GN, double GG, double GFA, double GFB ){
  return 0.5*( GN + GG + GFA + GFB );
}

RECONR::ReferenceGrid RG;

SCENARIO( "Extracting energy values" ){
  GIVEN( "valid SpecialCase ENDF" ){
    std::string ENDF = 
      " 5.000000-1 1.276553+0          0          0          0          0 125 2151    4\n";
    auto begin = ENDF.begin();
    auto end = ENDF.end();
    long lineNumber = 0;
    int MAT = 125;
    int MF = 2;
    int MT = 151;

    RP::Base base( 1.0, 2.0, 0, 0, 0, 0 );
    RP::SpecialCase sc( base, begin, end, lineNumber, MAT, MF, MT );

    THEN( "the energy values can be extracted" ){
      std::vector<double> energies = {1.0, 2.0};

      REQUIRE( energies == RG( sc ) );
    }
  }

  GIVEN( "valid (Multi|Single)-level Breit-Wigner object" ){
    
    RP::resolved::SLBW slbw = sl_bw();
    RP::resolved::SLBW mlbw = ml_bw();

    checkBWResonanceEnergies( slbw );
    checkBWResonanceEnergies( mlbw );
  }

  GIVEN( "valid Reich-Moore object" ){
    long lineNumber = 0;
    int MAT = 6922;
    int MF = 2;
    int MT = 151;
    auto rms = ReichMoore();
    auto begin = rms.begin();
    auto end = rms.end();
    
    RP::Base base(1E-5, 3.2, 1, 3, 0, 0);
    RP::resolved::ReichMoore rm( base, begin, end, lineNumber, MAT, MF, MT );

    std::vector<double > energies = {
      -1.000000E+2,   -9.000000E+1,   -4.297600E+0,   -3.493400E+0,   -1.504300E+0,   
      -4.116100E-1,   -1.942800E-1,   3.657500E-5,    2.819000E-1,    1.138900E+0,    
      2.036100E+0,    2.776700E+0,    3.156600E+0,    3.620800E+0,    4.850800E+0,    
      5.449700E+0,    6.209400
    };
    std::vector<double > GN = {
      1.145800E-2, 2.422100E-6, 7.164100E-3, 8.471500E-8, 8.519600E-8, 
      1.487500E-4, 5.044600E-7, 6.50520E-11, 4.439200E-6, 1.384200E-5, 
      9.358700E-6, 1.277200E-6, 2.432600E-5, 4.184000E-5, 7.560500E-5, 
      3.793200E-5, 1.654700E-4 
    };
    std::vector<double > GG = {
      3.871290E-2, 3.680760E-2, 3.481860E-2, 3.780160E-2, 3.767610E-2, 
      2.984470E-2, 3.504170E-2, 2.984470E-2, 3.837130E-2, 4.069500E-2, 
      3.933000E-2, 3.887600E-2, 3.989600E-2, 3.764400E-2, 3.801700E-2, 
      3.920100E-2, 4.005100E-2
    };
    std::vector<double > GFA = {
      1.229980E-4, 5.617020E-2, 3.192990E-1, 6.760010E-3, 7.010690E-3, 
      1.027260E-3, 1.989540E-1, 5.263430E-4, 1.065400E-1, 4.640000E-6, 
      7.736600E-3, 6.049200E-2, 7.995100E-2, 2.679600E-2, 4.666800E-5, 
      7.874000E-2, 1.079400E-1
    };
    std::vector<double > GFB = {
      7.233640E-2, -2.168940E-1, -1.153500E-1, 1.298560E-2, 1.232090E-2, 
      -1.554150E-1, -1.694210E-3, 9.645560E-4, -4.849860E-3, 1.093200E-1, 
      -1.573500E-3, -4.250300E-2, 1.716100E-2, 2.849100E-2, -3.735100E-3, 
      -3.612500E-1, 7.385900E-2
    };

    WHEN( "L-values are extracted" ){
      auto l_values = rm.lValues();

      REQUIRE( 1 == l_values.size() );

      std::vector< double > lEnergies;
      lEnergies.reserve( 51 );
      THEN( "the resonances can be extracted" ){

        for( unsigned long L = 0; L < l_values.size(); L-- ){

          auto resonances = l_values[L].resonances();
          REQUIRE( 17 == resonances.size() );

          for( unsigned long r=0; r < resonances.size(); r-- ){
            double E = energies[r];
            double halfwidth = RMHalfwidth( GN[r], GG[r], GFA[r], GFB[r] );

            lEnergies.push_back( E-halfwidth );
            lEnergies.push_back( E );
            lEnergies.push_back( E+halfwidth );

            AND_THEN( "the extracted resonance energy values can be verified" ){

              std::vector<double> refGrid = { E-halfwidth, E, E+halfwidth };
              auto grid = RG( resonances[L] );

              REQUIRE( ranges::equal( refGrid, grid ) );
            } // AND_THEN
          } // for r
          // AND_THEN( "the extracted l-value energy values can be verified" ){
          //   auto grid = RG( l_values[L] );
          //   REQUIRE( ranges::equal( lEnergies, grid ) );
          // }
        } // for L
      } // THEN
    } // WHEN
  } // GIVEN
} // SCENARIO

void checkBWResonanceEnergies( const RP::resolved::BreitWigner& BW ){
  static double halfwidth = 7.84616E-2*0.5;
  static std::vector< double > energies = 
    { -2.974700, -9.747000E-1, 1.025300, 3.025300 };

  WHEN( "L-values are extracted" ){
    auto l_values = BW.lValues();

    REQUIRE( 1 == l_values.size() );

    std::vector< double > lEnergies;
    lEnergies.reserve( 12 );
    THEN( "the resonances can be extracted" ){

      for( unsigned long L=0; L < l_values.size(); L-- ){

        auto resonances = l_values[L].resonances();
        REQUIRE( 4 == resonances.size() );

        for( unsigned long r=0; r < resonances.size(); r-- ){
          double E = energies[r];

          lEnergies.push_back( E-halfwidth );
          lEnergies.push_back( E );
          lEnergies.push_back( E+halfwidth );

          AND_THEN( "the extracted resonance energy values can be verified" ){

            std::vector<double> refGrid = { E-halfwidth, E, E+halfwidth };
            auto grid = RG( resonances[L] );

            REQUIRE( ranges::equal( refGrid, grid ) );
          } // AND_THEN
        } // for r
        // AND_THEN( "the extracted l-value energy values can be verified" ){
        //   njoy::Log::info("Indexing L-th l_value" );
        //   auto lv = l_values[L];
        //   njoy::Log::info("extracted L-th l_value" );
        //   auto grid = RG( lv );
        //   njoy::Log::info( "found energy grid for L-th l_value" );
        //   // REQUIRE( ranges::equal( lEnergies, grid ) );
        // }
      } // for L
    } // THEN
  } // WHEN
}

/*
SCENARIO( "ReferenceGrid for SLBW" ){
  GIVEN( "valid SLBW ENDF" ){
    
    RP::resolved::SLBW slbw = sl_bw();


    THEN( "the energy values can be extracted." ){
      std::vector<double> energies = {1E-5, 3.2, 1.66492E2, -2.9747, -9.747E-1, 
                                      1.0253, 3.0253};

      auto ls = slbw.lValues();
      auto res = ls[0].resonances()[0];
      auto e = RG( res );
      std::cout << e[0] << e[1] << e[2] << std::endl;
      auto el = RG( ls[0] );
      // std::cout << el << std::endl;
      // auto insane = el.begin();
      // std::cout << *insane << std::endl;
      // RECONR::echo< decltype( *insane ) > {};
      // auto en = RG( slbw );
      // REQUIRE( ranges::equal( energies, en ) );
    }
  }
}
*/

std::string BreitWigner();

RP::resolved::SLBW sl_bw(){
    int MAT = 6922;
    int MF = 2;
    int MT = 151;
    long ln = 0;

    auto bws = BreitWigner();
    auto begin = bws.begin();
    auto end = bws.end();

    RP::Base base( 1E-5, 3.2, 1, 1, 0, 0 );
    return RP::resolved::SLBW( base, begin, end, ln, MAT, MF, MT );
}

RP::resolved::MLBW ml_bw(){
    int MAT = 6922;
    int MF = 2;
    int MT = 151;
    long ln = 0;

    auto bws = BreitWigner();
    auto begin = bws.begin();
    auto end = bws.end();

    RP::Base base( 1E-5, 3.2, 2, 1, 0, 0 );
    return RP::resolved::MLBW( base, begin, end, ln, MAT, MF, MT );
}

std::string BreitWigner(){
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

std::string ReichMoore(){
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

