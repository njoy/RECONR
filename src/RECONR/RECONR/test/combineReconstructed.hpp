SCENARIO( "Testing combineReconstructed" ){
  double absTol{ 1E-6 };
  double relTol{ 1E-1 }; // This tolerance is large by design

  using RPair = njoy::RECONR::XSPair;
  using PPair = njoy::RECONR::PPair;

  GIVEN( "an SLBW object" ){
    std::set< double > userSupplied{ 1.0, 2.0, 3.0 };
    auto material = details::ENDFMaterial( "SLBW", 1 );
    auto r2d2 = njoy::RECONR::R2D2::Factory()( logger, material );

    auto projectile = r2d2.projectile();
    auto target = r2d2.target();

    tRECONR::linearizeXS( logger, r2d2, absTol, relTol);
    auto refGrid = tRECONR::unionizeEnergyGrid( logger, r2d2, userSupplied );

    tRECONR::reconstructResonances( logger, refGrid, r2d2, relTol, absTol );

    auto energies = tRECONR::unionizeEnergyGrid( logger, r2d2, refGrid );

    tRECONR::reconstructCrossSections( logger, r2d2, energies );

    const auto preReactions = r2d2.reactions();
    auto& reconstructed = r2d2.reconstructedResonances();

    tRECONR::combineReconstructed( logger, r2d2, energies );

    auto sizeEnergies = ranges::distance( energies );
    auto& reactions = r2d2.reactions();

    THEN( "the keys can be verified" ){
      std::vector< njoy::RECONR::ReactionID > refReactions{ 
        ReactionID{ projectile, target, ReactionType{ 1 } },
        ReactionID{ projectile, target, ReactionType{ 2 } },
        ReactionID{ projectile, target, ReactionType{ 18 } },
        ReactionID{ projectile, target, ReactionType{ 51 } },
        ReactionID{ projectile, target, ReactionType{ 52 } },
        ReactionID{ projectile, target, ReactionType{ 102 } },
        ReactionID{ projectile, target, ReactionType{ 875 } },
        ReactionID{ projectile, target, ReactionType{ 876 } },
        ReactionID{ projectile, target, ReactionType{ 877 } }
      };

      auto rxKeys = ranges::view::keys( reactions ) | ranges::to_vector;
      std::sort( refReactions.begin(), refReactions.end() );
      std::sort( rxKeys.begin(), rxKeys.end() );

      CHECK( ranges::equal( refReactions, rxKeys ) );

      std::vector< njoy::RECONR::ReactionID > refReconstructed{ 
        ReactionID{ projectile, target, ReactionType{ 2 } },
        ReactionID{ projectile, target, ReactionType{ 19 } },
        ReactionID{ projectile, target, ReactionType{ 102 } }
      };

      auto rnKeys = ranges::view::keys( reconstructed ) | ranges::to_vector;
      std::sort( refReconstructed.begin(), refReconstructed.end() );
      std::sort( rnKeys.begin(), rnKeys.end() );

      CHECK( ranges::equal( refReconstructed, rnKeys ) );


    } // THEN
    THEN( "combined elastic cross section can be verified" ){
      ReactionID elastic{ projectile, target, ReactionType{ 2 } };

      auto rx = preReactions.at( elastic ).
          template crossSections< RPair >().second;
      auto rn = energies | ranges::view::transform( 
          reconstructed.at( elastic ).front() )
          | ranges::to_vector;

      std::vector< double > refXS = sumRanges( rx, rn );

      auto reaction = reactions.at( elastic );
      details::checkRanges( refXS, reaction.crossSections< RPair >().second );
      
    } // THEN
    THEN( "combined fission cross section can be verified" ){
      ReactionID fissionT{ projectile, target, ReactionType{ 18 } };
      ReactionID fission{ projectile, target, ReactionType{ 19 } };

      auto rx = preReactions.at( fissionT ).
          template crossSections< RPair >().second;
      auto rn = energies | ranges::view::transform( 
          reconstructed.at( fission ).front() )
          | ranges::to_vector;

      std::vector< double > refXS = sumRanges( rx, rn );

      auto reaction = reactions.at( fissionT );
      details::checkRanges( refXS, reaction.crossSections< RPair >().second );
      
    } // THEN
    THEN( "combined capture cross section can be verified" ){
      ReactionID capture{ projectile, target, ReactionType{ 102 } };

      auto rx = preReactions.at( capture ).
          template crossSections< RPair >().second;
      auto rn = energies | ranges::view::transform( 
          reconstructed.at( capture ).front() )
          | ranges::to_vector;

      std::vector< double > refXS = sumRanges( rx, rn );

      auto reaction = reactions.at( capture );
      details::checkRanges( refXS, reaction.crossSections< RPair >().second );
      
    } // THEN
    
  } // GIVEN
  GIVEN( "an RM object" ){
    std::set< double > userSupplied{ 1.0, 2.0, 3.0 };
    auto material = details::ENDFMaterial( "RM", 0 );
    auto r2d2 = njoy::RECONR::R2D2::Factory()( logger, material );

    auto projectile = r2d2.projectile();
    auto target = r2d2.target();

    tRECONR::linearizeXS( logger, r2d2, absTol, relTol);
    auto refGrid = tRECONR::unionizeEnergyGrid( logger, r2d2, userSupplied );

    tRECONR::reconstructResonances( logger, refGrid, r2d2, relTol, absTol );

    auto energies = tRECONR::unionizeEnergyGrid( logger, r2d2, refGrid );

    tRECONR::reconstructCrossSections( logger, r2d2, energies );

    const auto preReactions = r2d2.reactions();
    const auto& reconstructed = r2d2.reconstructedResonances();

    tRECONR::combineReconstructed( logger, r2d2, energies );

    const auto& unresolved = r2d2.unresolved();

    auto sizeEnergies = ranges::distance( energies );
    auto& reactions = r2d2.reactions();

    THEN( "the keys can be verified" ){
      std::vector< njoy::RECONR::ReactionID > refReactions{ 
        ReactionID{ projectile, target, ReactionType{ 1 } },
        ReactionID{ projectile, target, ReactionType{ 2 } },
        ReactionID{ projectile, target, ReactionType{ 18 } },
        ReactionID{ projectile, target, ReactionType{ 51 } },
        ReactionID{ projectile, target, ReactionType{ 52 } },
        ReactionID{ projectile, target, ReactionType{ 102 } },
        ReactionID{ projectile, target, ReactionType{ 875 } },
        ReactionID{ projectile, target, ReactionType{ 876 } },
        ReactionID{ projectile, target, ReactionType{ 877 } }
      };

      auto rxKeys = ranges::view::keys( reactions ) | ranges::to_vector;
      std::sort( refReactions.begin(), refReactions.end() );
      std::sort( rxKeys.begin(), rxKeys.end() );

      CHECK( ranges::equal( refReactions, rxKeys ) );

      std::vector< njoy::RECONR::ReactionID > refReconstructed{ 
        ReactionID{ projectile, target, ReactionType{ 2 } },
        ReactionID{ projectile, target, ReactionType{ 19 } },
        ReactionID{ projectile, target, ReactionType{ 102 } }
      };

      auto rnKeys = ranges::view::keys( reconstructed ) | ranges::to_vector;
      std::sort( refReconstructed.begin(), refReconstructed.end() );
      std::sort( rnKeys.begin(), rnKeys.end() );

      CHECK( ranges::equal( refReconstructed, rnKeys ) );


    } // THEN
    THEN( "combined elastic cross section can be verified" ){
      ReactionID elastic{ projectile, target, ReactionType{ 2 } };

      auto rx = preReactions.at( elastic ).
          template crossSections< RPair >().second;
      auto rn = energies | ranges::view::transform( 
          reconstructed.at( elastic ).front() )
          | ranges::to_vector;

      std::vector< double > refXS = sumRanges( rx, rn );

      auto reaction = reactions.at( elastic );
      details::checkRanges( refXS, reaction.crossSections< RPair >().second );
    } // THEN
    THEN( "combined fission cross section can be verified" ){
      ReactionID fissionT{ projectile, target, ReactionType{ 18 } };
      ReactionID fission{ projectile, target, ReactionType{ 19 } };

      auto rx = preReactions.at( fissionT ).
          template crossSections< RPair >().second;
      auto rn = energies | ranges::view::transform( 
          reconstructed.at( fission ).front() )
          | ranges::to_vector;

      std::vector< double > refXS = sumRanges( rx, rn );

      auto reaction = reactions.at( fissionT );
      details::checkRanges( refXS, reaction.crossSections< RPair >().second );
      
    } // THEN
    THEN( "combined capture cross section can be verified" ){
      ReactionID capture{ projectile, target, ReactionType{ 102 } };

      auto rx = preReactions.at( capture ).
          template crossSections< RPair >().second;
      auto rn = energies | ranges::view::transform( 
          reconstructed.at( capture ).front() )
          | ranges::to_vector;

      std::vector< double > refXS = sumRanges( rx, rn );

      auto reaction = reactions.at( capture );
      details::checkRanges( refXS, reaction.crossSections< RPair >().second );
      
    } // THEN
    
  } // GIVEN
  GIVEN( "an RML object" ){
    std::set< double > userSupplied{ 1.0, 2.0, 3.0 };
    auto material = details::ENDFMaterial( "RML", 1 );
    auto r2d2 = njoy::RECONR::R2D2::Factory()( logger, material );

    auto projectile = r2d2.projectile();
    auto target = r2d2.target();

    tRECONR::linearizeXS( logger, r2d2, absTol, relTol);
    auto refGrid = tRECONR::unionizeEnergyGrid( logger, r2d2, userSupplied );

    tRECONR::reconstructResonances( logger, refGrid, r2d2, relTol, absTol );

    auto energies = tRECONR::unionizeEnergyGrid( logger, r2d2, refGrid );

    tRECONR::reconstructCrossSections( logger, r2d2, energies );

    const auto preReactions = r2d2.reactions();
    auto& reconstructed = r2d2.reconstructedResonances();

    tRECONR::combineReconstructed( logger, r2d2, energies );

    auto sizeEnergies = ranges::distance( energies );
    auto& reactions = r2d2.reactions();

    THEN( "the keys can be verified" ){
      std::vector< njoy::RECONR::ReactionID > refReactions{ 
        ReactionID{ projectile, target, ReactionType{ 1 } },
        ReactionID{ projectile, target, ReactionType{ 2 } },
        ReactionID{ projectile, target, ReactionType{ 18 } },
        ReactionID{ projectile, target, ReactionType{ 51 } },
        ReactionID{ projectile, target, ReactionType{ 52 } },
        ReactionID{ projectile, target, ReactionType{ 102 } },
        ReactionID{ projectile, target, ReactionType{ 875 } },
        ReactionID{ projectile, target, ReactionType{ 876 } },
        ReactionID{ projectile, target, ReactionType{ 877 } }
      };

      auto rxKeys = ranges::view::keys( reactions ) | ranges::to_vector;
      std::sort( refReactions.begin(), refReactions.end() );
      std::sort( rxKeys.begin(), rxKeys.end() );

      CHECK( ranges::equal( refReactions, rxKeys ) );

      std::vector< njoy::RECONR::ReactionID > refReconstructed{ 
        ReactionID{ projectile, target, ReactionType{ 2 } },
        // ReactionID{ projectile, target, ReactionType{ 19 } },
        ReactionID{ projectile, target, ReactionType{ 102 } }
      };

      auto rnKeys = ranges::view::keys( reconstructed ) | ranges::to_vector;
      std::sort( refReconstructed.begin(), refReconstructed.end() );
      std::sort( rnKeys.begin(), rnKeys.end() );

      CHECK( ranges::equal( refReconstructed, rnKeys ) );


    } // THEN

    THEN( "combined elastic cross section can be verified" ){
      ReactionID elastic{ projectile, target, ReactionType{ 2 } };

      auto rx = preReactions.at( elastic ).
          template crossSections< RPair >().second;
      auto rn = energies | ranges::view::transform( 
          reconstructed.at( elastic ).front() )
          | ranges::to_vector;

      std::vector< double > refXS = sumRanges( rx, rn );

      auto reaction = reactions.at( elastic );
      details::checkRanges( refXS, reaction.crossSections< RPair >().second );
      
    } // THEN
    THEN( "combined capture cross section can be verified" ){
      ReactionID capture{ projectile, target, ReactionType{ 102 } };

      auto rx = preReactions.at( capture ).
          template crossSections< RPair >().second;
      auto rn = energies | ranges::view::transform( 
          reconstructed.at( capture ).front() )
          | ranges::to_vector;

      std::vector< double > refXS = sumRanges( rx, rn );

      auto reaction = reactions.at( capture );
      details::checkRanges( refXS, reaction.crossSections< RPair >().second );
      
    } // THEN
    
  } // GIVEN
} // SCENARIO
