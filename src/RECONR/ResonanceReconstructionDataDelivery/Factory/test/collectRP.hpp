SCENARIO( "Testing the collection of resonance parameter data" ){
  GIVEN( "ENDF Material with SLBW" ){
    auto material = details::ENDFMaterial( "SLBW" );

    WHEN( "the resonance parameters are extracted" ){
      auto resParams = std::get< 0 >( TFactory::collectRP( material ) );

      THEN( "we can verify a few things" ){

        CHECK( 4.5105E4 == resParams.ZA() );
        CHECK( 104.0 == resParams.atomicWeightRatio() );

        auto isos = resParams.isotopes();
        // Note, I'm not testing everything because I trust ENDFtk to do its
        // job
        CHECK( 1 == ranges::distance( isos ) );
      } // THEN
    } // WHEN
    
  } // GIVEN
  GIVEN( "ENDF Material with RM" ){
    auto material = details::ENDFMaterial( "RM" );

    WHEN( "the resonance parameters are extracted" ){
      auto resParams = std::get< 0 >( TFactory::collectRP( material ) );

      THEN( "we can verify a few things" ){

        CHECK( 4.5105E4 == resParams.ZA() );
        CHECK( 104.0 == resParams.atomicWeightRatio() );

        auto isos = resParams.isotopes();
        // Note, I'm not testing everything because I trust ENDFtk to do its
        // job
        CHECK( 1 == ranges::distance( isos ) );
      } // THEN
    } // WHEN
    
  } // GIVEN
  GIVEN( "ENDF Material with RML" ){
    auto material = details::ENDFMaterial( "RML" );

    WHEN( "the resonance parameters are extracted" ){
      auto resParams = std::get< 0 >( TFactory::collectRP( material ) );

      THEN( "we can verify a few things" ){

        CHECK( 4.5105E4 == resParams.ZA() );
        CHECK( 104.0 == resParams.atomicWeightRatio() );

        auto isos = resParams.isotopes();
        // Note, I'm not testing everything because I trust ENDFtk to do its
        // job
        CHECK( 1 == ranges::distance( isos ) );
      } // THEN
    } // WHEN
  } // GIVEN
} // SCENARIO
