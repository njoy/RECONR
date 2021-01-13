static
std::vector< double > getResonanceReferenceGrid( 
    const RPVariant& parameters,
    const elementary::ParticleID& target,
    const elementary::ParticleID& proj ){
  return std::visit(
    [&]( auto&& arg ){ 
      return ResonanceReconstructionDataDelivery::getResonanceReferenceGrid(
          arg, target, proj ); 
    }, 
    parameters 
  );
}

static
std::vector< double > getResonanceReferenceGrid( 
    const ENDFtk::section::Type< 2, 151 >& parameters,
    const elementary::ParticleID& target,
    const elementary::ParticleID& proj ){

  std::vector< double > grid{};
  for( const auto& iso : parameters.isotopes() ){
    auto rg = referenceGrid( iso, target, proj );
    grid |= ranges::action::push_back( rg );
  }
  
  return grid;
}
