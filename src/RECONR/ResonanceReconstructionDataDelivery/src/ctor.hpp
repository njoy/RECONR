ResonanceReconstructionDataDelivery( 
    elementary::ParticleID&& projectile,
    elementary::ParticleID&& target,
    Info_t&& info,
    std::vector< int > mts,
    XSMap_t&& reactions,
    PPMap_t&& pProductions,
    // PYMap_t&& pYields,
    RPVariant&& resonanceParameters,
    std::pair< std::optional< Range_t >, std::optional< Range_t > >&& resRanges
    )
  try:
    projectile_( std::move( projectile ) ),
    target_( std::move( target ) ),
    information_( std::move( info ) ),
    MF3MTs_( std::move( mts ) ),
    reactions_( std::move( reactions ) ),
    photonProductions_( std::move( pProductions ) ),
    // photonYields_( std::move( pYields ) ),
    resonanceParameters_( std::move( resonanceParameters ) ),
    resonanceReferenceGrid_( 
      getResonanceReferenceGrid( this->resonanceParameters_, 
                                 this->target_, this->projectile_  ) ),
    resolvedRange_( std::move( resRanges.first ) ),
    unresolvedRange_( std::move( resRanges.second ) )
  { } catch( ... ){
    Log::info( "Trouble creating an R2D2 object." );
    throw;
  }

