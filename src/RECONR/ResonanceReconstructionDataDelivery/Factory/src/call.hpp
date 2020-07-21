ResonanceReconstructionDataDelivery operator()( const Material_t& material ){

  auto info = Factory::information( material );
  auto target = Factory::target( info );
  auto projectile = Factory::projectile( info );
  
  return ResonanceReconstructionDataDelivery{ 
    std::move( target  ),
    std::move( projectile ),
    std::move( info ),
    Factory::collectXS(   material, projectile, target ),
    Factory::collectPPXS( material, projectile, target ),
    Factory::collectRP(   material ) 
  };
}
