/*
 * This method is used *before* resonances are reconstructed
 */
static
std::vector< double > unionizeEnergyGrid( const Logger& logger,
                         const R2D2& r2d2,
                         const std::set< double >& user ){

  const auto& reactions = r2d2.reactions();
  const auto& ppReactions = r2d2.photonProductions();
  const auto& resonanceGrid = r2d2.resonanceReferenceGrid();

  std::vector< double > grid{ resonanceGrid.begin(), resonanceGrid.end() };
  logger.first << 
    "\nGenerating unionized energy grid prior to reconstructing resonances"
         << std::endl;
  grid |= ranges::action::push_back( user );

  for( const auto& [ID, reaction] : reactions ){
    logger.first << fmt::format( "{}", ID.symbol() ) << std::endl;

    grid |= ranges::action::push_back( 
      reaction.crossSections< interp::LinearTable >().x() );
  }

  logger.first <<  "Photon productions" << std::endl;
  for( const auto& [ ID, reaction ] : ppReactions ){
    logger.first << fmt::format( "{}", ID.symbol() ) << std::endl;
    for( const auto& discrete : reaction.productions< interp::LinearTable >() ){
      grid |= ranges::action::push_back( discrete.x() );
    }
  }

  logger.first << "Photon yields" << std::endl;
  for( const auto& [ ID, yields ] : r2d2.photonYields() ){
    logger.first << fmt::format( "{}", ID.symbol() ) << std::endl;
    for( const auto& discrete : yields.yields< interp::LinearTable >() ){
      grid |= ranges::action::push_back( discrete.x() );
    }
  }

  std::sort( grid.begin(), grid.end() );
  auto last = std::unique( grid.begin(), grid.end(), 
    []( auto&& l, auto&& r ){ 
      return utility::sigfig( l ) == utility::sigfig( r ); } );

  grid.erase( last, grid.end() );

  return grid 
    | ranges::view::filter( []( auto&& e ){ return e != 0.0; } )
    | ranges::to_vector;
}

/*
 * This method is used *after* resonances are reconstructed
 */
static
auto unionizeEnergyGrid( 
  const Logger& logger,
  R2D2& r2d2,
  std::vector< double >& grid ){
  logger.first << 
    "\nGenerating unionized energy grid after reconstructing resonances"
         << std::endl;

  for( const auto& [ID, V] : r2d2.reconstructedResonances() ){
    for( const auto& XS : V ){
      grid |= ranges::action::push_back( XS.x() );
    }
  }

  for( const auto& [ID, U] : r2d2.unresolved() ){
    auto& XS = U.crossSections< interp::LinearLinear >();
    grid |= ranges::action::push_back( XS.x() );
  }

  const auto& resolvedBoundaries = r2d2.resolvedRange();
  if( resolvedBoundaries ){
    grid |= ranges::action::push_back( {
      utility::sigfig( resolvedBoundaries->first,  9, -1 ),
      utility::sigfig( resolvedBoundaries->first,  9, +1 ),
      utility::sigfig( resolvedBoundaries->second, 9, -1 ),
      utility::sigfig( resolvedBoundaries->second, 9, +1 ),
      } );
  }
  const auto& unresolvedBoundaries = r2d2.unresolvedRange();
  if( unresolvedBoundaries ){
    grid |= ranges::action::push_back( {
      utility::sigfig( unresolvedBoundaries->first,  9, -1 ),
      utility::sigfig( unresolvedBoundaries->first,  9, +1 ),
      utility::sigfig( unresolvedBoundaries->second, 9, -1 ),
      utility::sigfig( unresolvedBoundaries->second, 9, +1 ),
      } );
  }
  // Remove duplicate values
  std::sort( grid.begin(), grid.end() );
  std::unique( grid.begin(), grid.end(), 
    []( auto&& l, auto&& r ){ 
      return utility::sigfig( l, 9, 0 ) == 
             utility::sigfig( r, 9, 0 ); } );

  // Find energies at resonance boundaries and nudge them.
  // resolved resonances
  if( resolvedBoundaries ){
    auto found = ranges::find( grid, resolvedBoundaries->first );
    if( found != grid.end() ){ *found = utility::sigfig( *found, 9, -1 ); }
    found = ranges::find( grid, resolvedBoundaries->second );
    if( found != grid.end() ){ *found = utility::sigfig( *found, 9, +1 ); }
  }

  // unresolved resonances
  if( unresolvedBoundaries ){
    auto found = ranges::find( grid, unresolvedBoundaries->first );
    if( found != grid.end() ){ *found = utility::sigfig( *found, 9, -1 ); }
    found = ranges::find( grid, unresolvedBoundaries->second );
    if( found != grid.end() ){ *found = utility::sigfig( *found, 9, +1 ); }
  }

  // Remove duplicate values
  std::sort( grid.begin(), grid.end() );
  auto last = std::unique( grid.begin(), grid.end(), 
    []( auto&& l, auto&& r ){ 
      return utility::sigfig( l, 9, 0 ) == 
             utility::sigfig( r, 9, 0 ); } );

  grid.erase( last, grid.end() );

  return grid 
    | ranges::view::filter( []( auto&& e ){ return e != 0.0; } )
    | ranges::view::transform( 
      []( auto && e ){ return utility::sigfig( e, 9, 0 ); } )
    | ranges::to_vector;
}
