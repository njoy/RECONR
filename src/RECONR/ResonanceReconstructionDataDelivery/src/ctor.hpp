ResonanceReconstructionDataDelivery( 
    XSmap_t&& reactions,
    RPVariant&& resonanceParameters )
  try:
    reactions_( std::move( reactions ) ),
    linearReactions_(),
    resonanceParameters_( std::move( resonanceParameters ) ),
    resonanceReferenceGrid_( 
      getResonanceReferenceGrid( this->resonanceParameters_ ) )
  { } catch( ... ){
    Log::info( "Trouble creating an R2D2 object." );
    throw;
  }

