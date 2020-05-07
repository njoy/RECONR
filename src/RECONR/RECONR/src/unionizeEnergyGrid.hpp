template< typename Range >
static
auto unionizeEnergyGrid( ResonanceReconstructionDataDelivery& r2d2,
                         const Range& user ){
  std::vector< double > grid = r2d2.resonanceReferenceGrid();
  grid |= ranges::action::push_back( user );

  for( const auto& [MT, XS] : r2d2.linearCrossSections() ){
    grid |= ranges::action::push_back( XS.x() );
  }

  std::sort( grid.begin(), grid.end() );

  return grid 
    | ranges::view::unique 
    | ranges::to_vector;
}
