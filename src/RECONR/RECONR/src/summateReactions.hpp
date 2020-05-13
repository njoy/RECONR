template< typename Range >
static
auto summateReactions( ResonanceReconstructionDataDelivery& r2d2,
                       Range& energies ){
  std::map< int, std::vector< double > > reactions;
  
  const auto linear = r2d2.linearCrossSections();
  const auto reconstructed = r2d2.reconstructedResonances();

  auto keys = ranges::view::keys( linear );
  auto recKeys = ranges::view::keys( reconstructed );

  for( const auto& MT : ranges::view::reverse( keys ) ){
  // for( const auto& [ MT, XS ] : linear ){
    const auto& XS = linear.at( MT );

    if( ENDFtk::isRedundant( MT ) ){
      
    }
    Log::info( "MT: {}", MT );
    auto y = energies 
      | ranges::view::transform( XS )
      | ranges::to_vector;
  }

  return reactions;
}
