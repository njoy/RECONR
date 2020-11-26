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
    auto x = reaction.crossSections< interp::LinearTable >().x();
    Log::info( "ID: {:20}\tsize: {}", ID.symbol(), ranges::distance( x ) );

    // grid |= ranges::action::push_back( 
    //   reaction.crossSections< interp::LinearTable >().x() );
    grid |= ranges::action::push_back( x );
  }

  Log::info( "Photon productions" );
  for( const auto& [ ID, reaction ] : ppReactions ){
    Log::info( "{}", ID.symbol() );
    for( const auto& discrete : reaction.productions< interp::LinearTable >() ){
      grid |= ranges::action::push_back( discrete.x() );
    }
  }

  Log::info( "Photon yields" );
  for( const auto& [ ID, yields ] : r2d2.photonYields() ){
    Log::info( "{}", ID.symbol() );
    for( const auto& discrete : yields.yields< interp::LinearTable >() ){
      grid |= ranges::action::push_back( discrete.x() );
    }
  }

  ranges::sort( grid );

  return grid 
    | ranges::view::filter( []( auto&& e ){ return e != 0.0; } )
    | ranges::view::unique
    | ranges::to_vector;
}

/*
 * This method is used *after* resonances are reconstructed
 */
static
auto unionizeEnergyGrid( const Logger& logger,
                         std::vector< double >& grid,
                         const R2D2::Range_t& resolvedBoundaries,
                         const R2D2::Range_t& unresolvedBoundaries,
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

  // Find energies at resonance boundaries and nudge them.
  // resolved resonances
  auto found = std::find( std::begin(grid), std::end(grid), 
                          resolvedBoundaries.first );
  if( found != grid.end() ){ *found = utility::sigfig( *found, 9, -1 ); }
  found = std::find( std::begin(grid), std::end(grid), 
                     resolvedBoundaries.second );
  if( found != grid.end() ){ *found = utility::sigfig( *found, 9, +1 ); }
  // unresolved resonances
  found = std::find( std::begin(grid), std::end(grid), 
                     unresolvedBoundaries.first );
  if( found != grid.end() ){ *found = utility::sigfig( *found, 9, -1 ); }
  found = std::find( std::begin(grid), std::end(grid), 
                     unresolvedBoundaries.second );
  if( found != grid.end() ){ *found = utility::sigfig( *found, 9, +1 ); }


  ranges::sort( grid );
  return grid 
    | ranges::view::filter( []( auto&& e ){ return e != 0.0; } )
    | ranges::view::unique
    | ranges::to_vector;
}
