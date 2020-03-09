ResonanceReconstructionDataDelivery operator()(){

  return ResonanceReconstructionDataDelivery{ 
    Factory::collectXS( this->material_ ) };
}
