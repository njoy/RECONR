ResonanceReconstructionDataDelivery( ENDFMaterial_t&& material)
  try:
    material( std::move( material ) ),
    crossSections_( collectXS() )
  { } catch( ... ){
    Log::info( "Trouble creating an R2D2 object." );
    throw;
  }

