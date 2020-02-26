ResonanceReconstructionDataDelivery operator()(){

  return ResonanceReconstructionDataDelivery{ 
    this->collectXS( this->material_ ) };
}
