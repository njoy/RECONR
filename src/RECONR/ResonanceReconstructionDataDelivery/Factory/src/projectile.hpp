static
elementary::ParticleID projectile( 
    const ENDFtk::section::Type< 1, 451 >& info ){

  switch ( info.NSUB() ) {
    case 10:
      return elementary::ParticleID( elementary::FundamentalParticleID( "n" ) );
      break;
    case 10010:
      return elementary::ParticleID( elementary::NuclideID( 1, 1, 0 ) );
      break;
    default:
      Log::info( "I don't know how to make a particle ID out of: {}.", 
                 info.NSUB() );
      throw std::exception();
      
  }
}

static
auto projectile( const Info_t& info ){
  return std::visit(
    []( auto&& i ){ return Factory::projectile( i ); },
    info
  );
}
