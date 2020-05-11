/*
 * This method is used *before* resonances are reconstructed
 */
template< typename Range >
static
auto unionizeEnergyGrid( ResonanceReconstructionDataDelivery& r2d2,
                         const Range& user ){
  std::vector< double > grid = r2d2.resonanceReferenceGrid();
  grid |= ranges::action::push_back( user );

  for( const auto& [MT, XS] : r2d2.linearCrossSections() ){
    grid |= ranges::action::push_back( XS.x() );
  }

  ranges::sort( grid );

  return grid 
    | ranges::view::unique 
    | ranges::to_vector;
}

/*
 * This method is used *after* resonances are reconstructed
 */
static
auto unionizeEnergyGrid( ResonanceReconstructionDataDelivery& r2d2 ){
  std::vector< double > grid;

  for( const auto& [MT, XS] : r2d2.linearCrossSections() ){
    grid |= ranges::action::push_back( XS.x() );
  }
  for( const auto& [MT, V] : r2d2.reconstructedResonances() ){
    for( const auto& XS : V ){
      grid |= ranges::action::push_back( XS.x() );
    }
  }

  ranges::sort( grid );

  return grid 
    | ranges::view::unique 
    | ranges::to_vector;
}
