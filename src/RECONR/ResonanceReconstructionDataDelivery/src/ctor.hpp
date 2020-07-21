ResonanceReconstructionDataDelivery( 
    elementary::ParticleID&& projectile,
    elementary::ParticleID&& target,
    Info_t&& info,
    XSMap_t&& reactions,
    PPMap_t&& pProductions,
    RPVariant&& resonanceParameters
    )
  try:
    projectile_( std::move( projectile ) ),
    target_( std::move( target ) ),
    information_( std::move( info ) ),
    reactions_( std::move( reactions ) ),
    photonProductions_( std::move( pProductions ) ),
    resonanceParameters_( std::move( resonanceParameters ) ),
    resonanceReferenceGrid_( 
      getResonanceReferenceGrid( this->resonanceParameters_ ) )
  { } catch( ... ){
    Log::info( "Trouble creating an R2D2 object." );
    throw;
  }

