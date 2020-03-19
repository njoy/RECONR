ResonanceReconstructionDataDelivery( XSmap_t&& crossSections )
  try:
    crossSections_( std::move( crossSections ) )
  { } catch( ... ){
    Log::info( "Trouble creating an R2D2 object." );
    throw;
  }

