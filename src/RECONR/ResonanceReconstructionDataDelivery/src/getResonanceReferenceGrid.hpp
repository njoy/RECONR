static
std::vector< double > getResonanceReferenceGrid( const RPVariant& parameters ){
  return std::visit(
    [&]( auto&& arg ){ 
      return ResonanceReconstructionDataDelivery::getResonanceReferenceGrid(
          arg ); 
    }, 
    parameters 
  );
}

static
std::vector< double > getResonanceReferenceGrid( 
    const ENDFtk::section::Type< 2, 151 >& parameters ){

  std::vector< double > grid{};
  for( const auto& iso : parameters.isotopes() ){
    auto rg = referenceGrid( iso );
    grid |= ranges::action::push_back( rg );
  }
  
  return grid;
}
