static
elementary::ParticleID target( 
    const ENDFtk::section::Type< 1, 451 >& info ){
  return elementary::ParticleID{
    elementary::NuclideID{ info.ZA(), info.LIS() } };
}

static
auto target( const Info_t& info ){
  return std::visit(
    []( auto&& i ){ return Factory::target( i ); },
    info
  );
}
