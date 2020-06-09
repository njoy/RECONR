ResonanceReconstructionDataDelivery operator()(){

  return ResonanceReconstructionDataDelivery{ 
    Factory::collectXS( this->material_ ),
    Factory::collectPPXS( this->material_ ),
    Factory::collectRP( this->material_ ) };
}
