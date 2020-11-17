static
PYMap_t collectPYields( const Material_t& material,
                        const elementary::ParticleID& projectile,
                        const elementary::ParticleID& target ){
  return std::visit( 
    [&]( auto&& arg ){ 
      return Factory::collectPYields( arg, projectile, target ); },
    material );
}

static
PYMap_t collectPYields( const ENDFMaterial_t& material,
                        const elementary::ParticleID& projectile,
                        const elementary::ParticleID& target ){

  PYMap_t yields{};
  if( material.hasFileNumber( 12 ) ){
    auto MF12 = material.fileNumber( 12 ).parse< 12 >();

    for( const auto& section : MF12.sections() ){
      if( section.LO() == 1 ){
        PhotonYields ys{ section.ZA(), section.AWR(),
                        interp::MF12toInterpolation( section ) };
        
        elementary::ReactionID reactionID{ 
          utility::copy( projectile ), utility::copy( target ), section.MT() };
        yields.insert( 
          std::make_pair( std::move( reactionID ), std::move( ys ) ) 
        );
      }
    }
    
  }

  return yields;
}
