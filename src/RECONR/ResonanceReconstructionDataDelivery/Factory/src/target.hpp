static
elementary::ParticleID target( 
    const ENDFtk::section::Type< 1, 451 >& info ){

  const auto ZA = info.ZA();
  if( ZA == 1 ){
    return elementary::ParticleID{ "n" };
  } else{
    return elementary::ParticleID{ elementary::NuclideID{ ZA, info.LIS() } };
  }
}

static
auto target( const Info_t& info ){
  return std::visit(
    []( auto&& i ){ return Factory::target( i ); },
    info
  );
}
