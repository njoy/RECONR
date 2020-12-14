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
    RP::SLBW slbw = breitWigner();
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
    auto& parameters = std::get< RP::RM >( rm.parameters() );

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
      double upperEnergy{ 5.4497 };
      std::vector< double > ref {
        1.06389,  1.1389,   1.21391,  2.02109,  2.0361,   
        2.05111,  2.74827,  2.7767,   2.80513,  3.14517,  
        3.1566,   3.16803,  3.60111,  3.6208,   3.64049,  
        4.8336,   4.8508,   4.868,    5.24932,  5.4497
      };
      auto trial = referenceGrid( parameters, rm, target, proj )
          | ranges::to_vector;
      std::sort( trial.begin(), trial.end() );

      details::checkRanges( ref, trial );
    }
  }

  GIVEN( "a RMatrixLimited resolved region" ){
    RP::ResonanceRange rml = rMatrixLimited();
    auto& parameters = std::get< RP::RML >( rml.parameters() );

    
    std::vector< double > reference{
      7193.6,   7788,     8382.4,  51511.8,  51520,  
      51528.2,  53580.8,  53590,   53599.2
    };

    const auto grid = referenceGrid( parameters, rml, target, proj );

    details::checkRanges( reference, grid );
  } // GIVEN

  GIVEN( "a SpecialCase region" ){
    RP::ResonanceRange sc = specialCase();
    auto& parameters = std::get< RP::SpecialCase >( sc.parameters() );

    THEN( "the boundaries of the region will be extracted" ){
      auto grid = referenceGrid( parameters, sc, target, proj );
      CHECK( grid.size() == 0 );
    }
  }

  GIVEN( "a Case A unresolved region" ){
    RP::ResonanceRange ca = caseA();
    auto& parameters = std::get< RP::CaseA >( ca.parameters() );

    std::vector<double> refEnergies{
      23000, 23000, 25000, 29844.4, 35000, 40000, 
      47751.1, 57004.1, 68050.2, 81236.7, 96978.5
    };
    auto energies = referenceGrid( parameters, ca, target, proj );

    details::checkRanges( refEnergies, energies );
  }

  GIVEN( "a Case B unresolved region" ){
    RP::ResonanceRange cb = caseB();
    auto& parameters = std::get< RP::CaseB >( cb.parameters() );

    std::vector<double> refEnergies{
      5700,     6804.53,  8000,     9000,   10000,  
      11937.8,  14000,    16000,    18000,  20000,  
      23875.5,  28502.1,  34025.1,  40000
    };
    auto energies = referenceGrid( parameters, cb, target, proj );

    details::checkRanges( refEnergies, energies );
  }

  GIVEN( "a Case C unresolved region" ){
    RP::ResonanceRange cc = caseC();
    auto& parameters = std::get< RP::CaseC >( cc.parameters() );

    std::vector<double> refEnergies{
      6000,   7000,     8000,     9550.21,  11400.8,  
      13610,  16247.3,  19395.7,  23154.1,  27640.9,  
      32997,  39391.1,  47024.1,  56136.3,  67014.2,  
      80000,  95502.1
    };
    auto energies = referenceGrid( parameters, cc, target, proj );

    details::checkRanges( refEnergies, energies );
  }
}

