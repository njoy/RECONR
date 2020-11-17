SCENARIO( "Testing the collection of photon yields" ){
  GIVEN( "an ENDF Material" ){
    auto material = details::ENDFMaterial( "SLBW", false );

    WHEN( "the photon yields have been extracted" ){
      njoy::elementary::ParticleID proj{ "n" };
      njoy::elementary::ParticleID target{ "fe56" };
      auto yields = TFactory::collectPYields( material, proj, target );
      
      using PID = ParticleID;
      using NID = NucleusID;
      
      std::vector< njoy::RECONR::ReactionID > IDs{ 
        ReactionID{ proj, target, ReactionType{ 18 } },
      };
      auto keys = ranges::view::keys( yields ) | ranges::to_vector;
      std::sort( IDs.begin(), IDs.end() );
      std::sort( keys.begin(), keys.end() );

      CHECK( ranges::equal( IDs, keys ) );

      THEN( "MT=18 can be checked" ){
        ReactionID reactionID{ PID{ "n" }, PID{ "fe56" }, ReactionType{ 18 } };
        auto yield = yields.at( reactionID );

        CHECK( 94239 == yield.ZA() );
        CHECK( 236.9986 == yield.atomicWeightRatio() );

        std::vector< double > refE{
          1.0E-5, 1.0E+6, 2.0E+6, 3.0E+6, 4.0E+6,
          5.0E+6, 6.0E+6, 7.0E+6, 7.50E+6, 8.0E+6,
          9.0E+6, 1.0E+7, 1.10E+7, 1.20E+7, 1.30E+7,
          1.40E+7, 1.50E+7, 2.0E+7
        };
        std::vector< double > refY{
        7.558439E+0, 9.048899E+0, 9.284268E+0, 9.667948E+0, 1.005163E+1,
        1.047208E+1, 1.089253E+1, 1.053180E+1, 1.035144E+1, 1.047116E+1,
        1.071060E+1, 1.095005E+1, 1.118949E+1, 1.142894E+1, 1.166838E+1,
        1.190783E+1, 1.214727E+1, 1.334450E+1
        };

        auto yield1 = std::get< interp::LinLin >(
            yield.yields< std::vector< interp::Variant > >()[ 0 ][ 0 ] );
        
        details::checkRanges( refE, yield1.x() | ranges::to_vector );
        details::checkRanges( refY, yield1.y() | ranges::to_vector );
      } // THEN
    } // WHEN
    
  } // GIVEN
  
} // SCENARIO
