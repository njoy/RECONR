ResonanceReconstructionDataDelivery operator()( const Logger& logger,
                                                const Material_t& material ){

  auto info = Factory::information( material );
  auto target = Factory::target( info );
  auto projectile = Factory::projectile( info );
  
  auto resonanceParameters = Factory::collectRP( material );
  auto resRanges = Factory::collectResonanceRanges( resonanceParameters );
  return ResonanceReconstructionDataDelivery{ 
    std::move( projectile ),
    std::move( target  ),
    std::move( info ),
    Factory::MTs( material ),
    Factory::collectXS(   logger, material, info, projectile, target ),
    Factory::collectPPXS( material, projectile, target ),
    std::move( resonanceParameters ),
    std::move( resRanges )
  };
}
