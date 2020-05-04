ResonanceReconstructionDataDelivery( 
    XSmap_t&& crossSections,
    RPVariant&& resonanceParameters )
  try:
    crossSections_( std::move( crossSections ) ),
    linearCrossSections_(),
    resonanceParameters_( std::move( resonanceParameters ) ),
    resonanceReferenceGrid_( 
      getResonanceReferenceGrid( this->resonanceParameters_ ) )
  { } catch( ... ){
    Log::info( "Trouble creating an R2D2 object." );
    throw;
  }

