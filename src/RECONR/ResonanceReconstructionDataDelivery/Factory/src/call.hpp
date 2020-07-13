ResonanceReconstructionDataDelivery operator()( const Material_t& material ){

  return ResonanceReconstructionDataDelivery{ 
    Factory::collectXS(   material ),
    Factory::collectPPXS( material ),
    Factory::collectRP(   material ) 
  };
}
