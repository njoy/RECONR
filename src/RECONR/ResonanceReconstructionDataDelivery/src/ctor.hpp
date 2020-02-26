// ResonanceReconstructionDataDelivery( const ENDFMaterial_t& material)
//   try:
//     ResonanceReconstructionDataDelivery( Factory( material ) )
//   { } catch( ... ){
//     Log::info( "Trouble creating an R2D2 object." );
//     throw;
//   }
// ResonanceReconstructionDataDelivery( const ENDFMaterial_t& material)
//   try:
//     material( std::move( material ) ),
//     crossSections_( collectXS() )
//   { } catch( ... ){
//     Log::info( "Trouble creating an R2D2 object." );
//     throw;
//   }

ResonanceReconstructionDataDelivery( XSmap_t&& crossSections )
  try:
    crossSections_( crossSections )
  { } catch( ... ){
    Log::info( "Trouble creating an R2D2 object." );
    throw;
  }

