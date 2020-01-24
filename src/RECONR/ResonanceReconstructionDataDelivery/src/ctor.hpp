ResonanceReconstructionDataDelivery( Material_t&& material)
  try:
    material( material ),
    crossSections_( collectXS() )
  { } catch( ... ){
    Log::info( "Trouble creating an R2D2 object." );
    throw;
  }

