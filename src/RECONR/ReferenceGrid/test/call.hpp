SCENARIO( "Extracting the reference grid" ){
  using namespace njoy::RECONR;

  GIVEN( "an Isotope" ){

    const std::vector< double > resonanceEnergies { 
      0.00001, 0.9860692, 1.0253, 1.0645308, 2.9860692, 3.0253, 3.0645308, 7.5,
      nextafter( 7.5 ),
      1.5E4, 17906.6, 20000, 23875.5, 28502.1, 30000, 35813.3, 40000, 47751.1, 
      50000, 59688.8, 60000, 70000, 80000, 90000, 100000, 100000, 100000
    };
    RP::Isotope iso = isotope();

    auto trial = referenceGrid( iso );
    details::checkRanges( resonanceEnergies, trial );
    
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

    details::checkRanges( reference, grid );
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

