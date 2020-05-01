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

std::vector< double > getResonanceReferenceGrid( 
    const std::vector< ENDFtk::resonanceParameters::Isotope >& parameters ){
}
