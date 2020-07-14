/*
 * This method is used *before* resonances are reconstructed
 */
static
auto unionizeEnergyGrid( std::ostream& output,
                         const R2D2::XSMap_t& reactions,
                         const R2D2::PPMap_t& ppReactions,
                         const std::vector< double >& resonanceGrid,
                         const std::vector< double >& user ){

  std::vector< double > grid{ resonanceGrid.begin(), resonanceGrid.end() };
  output << 
    "\nGenerating unionized energy grid prior to reconstructing resonances"
         << std::endl;
  grid |= ranges::action::push_back( user );

  for( const auto& [ID, reaction] : reactions ){
    grid |= ranges::action::push_back( 
      reaction.crossSections< interp::LinearTable >().x() );
    grid |= ranges::action::push_back( std::abs( reaction.reactionQValue() ) );
  }

  for( const auto& [ ID, reaction ] : ppReactions ){
    for( const auto& discrete : reaction.productions< interp::LinearTable >() ){
      grid |= ranges::action::push_back( discrete.x() );
    }
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
auto unionizeEnergyGrid( std::ostream& output, 
                         std::vector< double >& grid,
                         const R2D2::ReconMap_t& resonances ){
  output << 
    "\nGenerating unionized energy grid after reconstructing resonances"
         << std::endl;

  for( const auto& [ID, V] : resonances ){
    for( const auto& XS : V ){
      grid |= ranges::action::push_back( XS.x() );
    }
  }
  ranges::sort( grid );

  return grid 
    | ranges::view::unique 
    | ranges::to_vector;
}
