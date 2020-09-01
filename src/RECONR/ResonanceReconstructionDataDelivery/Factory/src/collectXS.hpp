static
XSMap_t collectXS( const Logger& logger,
                   const Material_t& material, 
                   const elementary::ParticleID& projectile,
                   const elementary::ParticleID& target ){
  return std::visit( 
    [&](auto&& arg ){ 
      return Factory::collectXS( logger, arg, projectile, target ); },
    material 
  );
}

static
XSMap_t collectXS( const Logger& logger, 
                   const ENDFMaterial_t& material,
                   const elementary::ParticleID& projectile,
                   const elementary::ParticleID& target ){
  XSMap_t xs{};


  if( not material.hasFileNumber( 3 ) ){
    Log::error( "ENDF file does not have MF=3. Can't proceed." );
    throw std::exception();
  }

  logger.first << fmt::format( 
    "\nCollecting cross section data (MF=3) from an ENDF file\n" );
  auto MF3 = material.fileNumber( 3 ).parse< 3 >();
  for( auto& section : MF3.sections() ){
    auto MT = section.MT();
    if( not elementary::ReactionType::isRegistered( MT ) ){
      Log::warning( "Skipping invalid reaction number: {}", MT );
      continue;
    }
    elementary::ReactionID reactionID{ 
      utility::copy( projectile ), utility::copy( target ), section.MT() };

    logger.first << fmt::format( "\t{} {}\n", MT, reactionID.symbol() );

    if( ENDFtk::isRedundant( MT ) and ( MT != 1 ) ){
      auto redundants = ENDFtk::redundantReactions.at( MT )
        | ranges::view::filter(
          [&]( auto&& MT ){ return MF3.hasSection( MT ); } );
  
      if( ranges::distance( redundants ) > 0 ){ continue; }
    }
  
    Reaction reaction{ 
      section.ZA(),
      section.AWR(),
      section.QM(),
      section.QI(),
      section.LR(),
      interp::TAB1toInterpolation( section ) };
  
    xs.insert( 
      std::make_pair( std::move( reactionID ), std::move( reaction ) ) );
  }

  return xs;
}
