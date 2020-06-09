ResonanceReconstructionDataDelivery( 
    XSMap_t&& reactions,
    PPMap_t&& pProductions,
    RPVariant&& resonanceParameters )
  try:
    reactions_( std::forward< XSMap_t >( reactions ) ),
    photonProductions_( std::forward< PPMap_t >( pProductions ) ),
    linearReactions_(),
    linearPhotonProductions_(),
    resonanceParameters_( std::forward< RPVariant >( resonanceParameters ) ),
    resonanceReferenceGrid_( 
      getResonanceReferenceGrid( this->resonanceParameters_ ) )
  { } catch( ... ){
    Log::info( "Trouble creating an R2D2 object." );
    throw;
  }

