static
PPMap_t collectPPXS( const Material_t& material,
                     const elementary::ParticleID& projectile,
                     const elementary::ParticleID& target ){
  return std::visit( 
    [&]( auto&& arg ){ 
      return Factory::collectPPXS( arg, projectile, target ); },
    material );
}

static
PPMap_t collectPPXS( const ENDFMaterial_t& material,
                     const elementary::ParticleID& projectile,
                     const elementary::ParticleID& target ){
  PPMap_t xs{};

  if( material.hasFileNumber( 13 ) ){
    auto MF13 = material.fileNumber( 13 ).parse< 13 >();
    for( const auto& section : MF13.sections() ){
      PPReaction reaction{
        section.ZA(), section.AWR(),
        interp::MF13toInterpolation( section )
      };
      elementary::ReactionID reactionID{ 
        utility::copy( projectile ), utility::copy( target ), section.MT() };
      xs.insert( std::make_pair( std::move( reactionID ), 
                                std::move( reaction ) ) 
               );
    }
  }  
  return xs;
}
