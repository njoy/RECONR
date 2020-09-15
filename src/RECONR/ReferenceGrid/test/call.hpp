SCENARIO( "Extracting the reference grid" ){
  using namespace njoy::RECONR;

  njoy::elementary::ParticleID proj{ "n" };
  njoy::elementary::ParticleID target{ "fe56" };
  GIVEN( "an Isotope" ){

    const std::vector< double > resonanceEnergies { 
      1e-05,    0.986069,  1.0253,   1.06453,  2.98607,  
      3.0253,   3.06453,   7.5,      15000,    17906.6,  
      21376.5,  25518.8,   30463.8,  36366.9,  43414,    
      51826.6,  61869.4,   73858.2,  88170.2,  100000
    };
    RP::Isotope iso = isotope();

    auto trial = referenceGrid( iso, target, proj );
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

    const auto trial = referenceGrid( resonance );
    CHECK( reference == trial );
  }

  GIVEN( "a Reich-Moore resolved region" ){
    RP::ResonanceRange rm = reichMoore();
    auto& parameters = std::get< 
      njoy::ENDFtk::resonanceParameters::resolved::ReichMoore >( 
        rm.parameters() );

    const std::vector< double > resonanceEnergies { 
      -1.000000E+2, -9.000000E+1, -4.297600E+0, -3.493400E+0,
      -1.504300E+0, -4.116100E-1, -1.942800E-1,  3.657500E-5,
       2.819000E-1,  1.138900E+0,  2.036100E+0,  2.776700E+0,
       3.156600E+0,  3.620800E+0,  4.850800E+0,  5.449700E+0 };

    GIVEN( "an l-value" ){
      const auto lValue = parameters.lValues().front();

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

          const auto trial = referenceGrid( resonance );
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
      const auto grid = referenceGrid( parameters, rm, target, proj );

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
    RP::ResonanceRange rml = rMatrixLimited();
    auto& parameters = std::get<
      njoy::ENDFtk::resonanceParameters::resolved::RMatrixLimited >( 
        rml.parameters() );

    
    std::vector< double > reference{
      1e-05,   7193.6,   7788,     8382.4,  51511.8,  
      51520,   51528.2,  53580.8,  53590,   53599.2,  
      550000
    };

    const auto grid = referenceGrid( parameters, rml, target, proj );

    details::checkRanges( reference, grid );
  } // GIVEN

  GIVEN( "a SpecialCase region" ){
    RP::ResonanceRange sc = specialCase();
    auto& parameters = std::get< 
      njoy::ENDFtk::resonanceParameters::SpecialCase >( sc.parameters() );

    THEN( "the boundaries of the region will be extracted" ){
      auto grid = referenceGrid( parameters, sc, target, proj );
      CHECK( grid.size() == 2 );
      CHECK( grid[ 0 ] == Approx( 1E-5 ) );
      CHECK( grid[ 1 ] == Approx( 1E+5 ) );
    }
  }

  /*
  GIVEN( "a Case A unresolved region" ){
    RP::unresolved::EnergyIndependent ca = caseA();

    std::vector<double> refEnergies{
      23.0000000000E3, 27.4568627590E3, 32.7773614170E3, 39.1288484370E3,
      46.7111052810E3, 55.7626263930E3, 66.5681208680E3, 79.4674677760E3,
      94.8664068070E3, 100E3 
    };
    auto energies = referenceGrid( ca, target, proj );

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
    auto energies = referenceGrid( cb, target, proj );

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
    auto energies = referenceGrid( cc, target, proj );

    RANGES_FOR( auto en, ranges::view::zip( refEnergies, energies ) ){
      auto ref = std::get< 0 >( en );
      auto trial = std::get< 1 >( en );
      CHECK( ref == Approx( trial ) );
    }
  }
  */
}

