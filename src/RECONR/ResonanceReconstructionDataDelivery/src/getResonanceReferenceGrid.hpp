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
    const std::vector< ENDFtk::resonanceParameters::Isotope >& parameters ){

  std::vector< double > grid{};
  for( const auto& iso : parameters ){
    auto rg = referenceGrid( iso );
    // Log::info( "rg: {}", rg | ranges::view::all );
    // grid |= ranges::action::push_back( rg );
  }
  
  return grid;
}
