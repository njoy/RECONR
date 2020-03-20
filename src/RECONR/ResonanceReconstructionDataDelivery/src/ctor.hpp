ResonanceReconstructionDataDelivery( XSmap_t&& crossSections )
  try:
    crossSections_( std::move( crossSections ) ),
    linearCrossSections_()
  { } catch( ... ){
    Log::info( "Trouble creating an R2D2 object." );
    throw;
  }

