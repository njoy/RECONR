ResonanceReconstructionDataDelivery( 
    XSMap_t&& reactions,
    PPMap_t&& pProductions,
    RPVariant&& resonanceParameters )
  try:
    reactions_( std::move( reactions ) ),
    photonProductions_( std::move( pProductions ) ),
    resonanceParameters_( std::move( resonanceParameters ) ),
    resonanceReferenceGrid_( 
      getResonanceReferenceGrid( this->resonanceParameters_ ) )
  { } catch( ... ){
    Log::info( "Trouble creating an R2D2 object." );
    throw;
  }

