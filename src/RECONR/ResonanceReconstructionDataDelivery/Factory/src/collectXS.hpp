static
XSMap_t collectXS( const Material_t& material, 
                   const elementary::ParticleID& projectile,
                   const elementary::ParticleID& target ){
  return std::visit( 
    [&](auto&& arg ){ return Factory::collectXS( arg, projectile, target ); },
    material 
  );
}

static
XSMap_t collectXS( const ENDFMaterial_t& material,
                   const elementary::ParticleID& projectile,
                   const elementary::ParticleID& target ){
  XSMap_t xs{};


  if( not material.hasFileNumber( 3 ) ){
    Log::error( "ENDF file does not have MF=3. Can't proceed." );
    throw std::exception();
  }

  if( material.hasFileNumber( 3 ) ){
    auto MF3 = material.fileNumber( 3 ).parse< 3 >();
    for( auto& section : MF3.sections() ){
      auto MT = section.MT();
      if( ENDFtk::isRedundant( MT ) ){
        auto redundants = ENDFtk::redundantReactions.at( MT )
          | ranges::view::filter(
            [&]( auto&& MT ){ return MF3.hasSection( MT ); } );
    
        if( ranges::distance( redundants ) > 0 ){
          continue;
        }
      }
    
      Reaction reaction{ 
        section.ZA(),
        section.AWR(),
        section.QM(),
        section.QI(),
        section.LR(),
        interp::TAB1toInterpolation( section ) };
    
      elementary::ReactionID reactionID{ 
        utility::copy( projectile ), utility::copy( target ), section.MT() };
      xs.insert( std::make_pair( std::move( reactionID ), 
                                 std::move( reaction ) ) 
               );
    }
  } else{
    Log::error( 
      "No MF=3 in ENDF file. Can't proceed without cross section data" );
    throw std::exception();
  }

  return xs;
}
