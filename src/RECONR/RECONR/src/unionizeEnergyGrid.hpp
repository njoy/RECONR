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
    logger.first << fmt::format( "{}", ID.symbol() ) << std::endl;
    // auto x = reaction.crossSections< interp::LinearTable >().x();
    // Log::info( "ID: {:20}\tsize: {}", ID.symbol(), ranges::distance( x ) );
    // grid |= ranges::action::push_back( x );

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
auto unionizeEnergyGrid( 
  const Logger& logger,
  std::vector< double >& grid,
  const std::optional< R2D2::Range_t >& resolvedBoundaries,
  const std::optional< R2D2::Range_t >& unresolvedBoundaries,
  const R2D2::ReconMap_t& resonances,
  const R2D2::UnresolvedMap_t& unresolved ){
  logger.first << 
    "\nGenerating unionized energy grid after reconstructing resonances"
         << std::endl;

  for( const auto& [ID, V] : resonances ){
    for( const auto& XS : V ){
      grid |= ranges::action::push_back( XS.x() );
    }
  }

  for( const auto& [ID, U] : unresolved ){
    auto& XS = U.crossSections< interp::LinearLinear >();
    grid |= ranges::action::push_back( XS.x() );
  }

  if( resolvedBoundaries ){
    grid |= ranges::action::push_back( {
      utility::sigfig( resolvedBoundaries->first,  9, -1 ),
      utility::sigfig( resolvedBoundaries->second, 9, +1 ),
      } );
  }
  if( unresolvedBoundaries ){
    grid |= ranges::action::push_back( {
      utility::sigfig( unresolvedBoundaries->first,  9, -1 ),
      utility::sigfig( unresolvedBoundaries->second, 9, +1 ),
      } );
  }
  ranges::sort( grid );

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


  ranges::sort( grid );
  return grid 
    | ranges::view::filter( []( auto&& e ){ return e != 0.0; } )
    | ranges::view::transform( 
      []( auto && e ){ return utility::sigfig( e, 9, 0 ); } )
    | ranges::view::unique
    | ranges::to_vector;
}
