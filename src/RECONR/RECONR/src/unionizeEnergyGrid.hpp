/*
 * This method is used *before* resonances are reconstructed
 */
static
auto unionizeEnergyGrid( const Logger& logger,
                         const R2D2& r2d2,
                         const std::vector< double >& user ){

  const auto& reactions = r2d2.reactions();
  const auto& ppReactions = r2d2.photonProductions();
  const auto& resonanceGrid = r2d2.resonanceReferenceGrid();

  std::vector< double > grid{ resonanceGrid.begin(), resonanceGrid.end() };
  logger.first << 
    "\nGenerating unionized energy grid prior to reconstructing resonances"
         << std::endl;
  grid |= ranges::action::push_back( user );

  for( const auto& [ID, reaction] : reactions ){
    grid |= ranges::action::push_back( 
      reaction.crossSections< interp::LinearTable >().x() );
  }

  for( const auto& [ ID, reaction ] : ppReactions ){
    for( const auto& discrete : reaction.productions< interp::LinearTable >() ){
      grid |= ranges::action::push_back( discrete.x() );
    }
  }

  ranges::sort( grid );

  return grid 
    | ranges::view::filter( []( auto&& e ){ return e != 0.0; } )
    | ranges::to_vector;
}

/*
 * This method is used *after* resonances are reconstructed
 */
static
auto unionizeEnergyGrid( const Logger& logger,
                         std::vector< double >& grid,
                         const R2D2::ReconMap_t& resonances ){
  logger.first << 
    "\nGenerating unionized energy grid after reconstructing resonances"
         << std::endl;

  for( const auto& [ID, V] : resonances ){
    for( const auto& XS : V ){
      grid |= ranges::action::push_back( XS.x() );
    }
  }
  ranges::sort( grid );

  // Find duplicate energy points and adjust (i.e., sigfig) to avoid
  // discontinuities
  // We know there are duplicates at the beginning and end. We don't want to
  // sigfig these values
  auto begin = std::find_if_not( grid.begin()+1, grid.end(),
    [&]( auto& E ){ return E == grid.front(); } );
  auto end = std::find_if_not( grid.rbegin()+1, grid.rend(),
    [&]( auto& E ){ return E == grid.back(); } );

  auto dupFound = std::adjacent_find( begin, end.base() );
  while( dupFound != end.base() ){

    auto value = *dupFound;
    *dupFound = utility::sigfig( value, 7, -1 );
    int i = 1;
    while( *( dupFound + i ) == value ){
      *( dupFound + i ) = utility::sigfig( value,7, +1 );
      i += 1;
    }

    dupFound = std::adjacent_find( dupFound + i, grid.end() );
  }

  ranges::sort( grid );
  return grid 
    | ranges::view::filter( []( auto&& e ){ return e != 0.0; } )
    | ranges::view::unique
    | ranges::to_vector;
}
