/*
 * This method is used *before* resonances are reconstructed
 */
template< typename Range >
static
auto unionizeEnergyGrid( std::ostream& output,
                         const R2D2::LinMap_t& reactions,
                         const R2D2::PPLinMap_t& ppReactions,
                         const std::vector< double >& grid,
                         Range& user ){

  output << 
    "\nGenerating unionized energy grid prior to reconstructing resonances"
         << std::endl;
  user |= ranges::action::push_back( grid );

  for( const auto& [MT, reaction] : reactions ){
    user |= ranges::action::push_back( reaction.crossSections().x() );
    user |= ranges::action::push_back( std::abs( reaction.reactionQValue() ) );
  }

  for( const auto& [ MT, reaction ] : ppReactions ){
    for( const auto& discrete : reaction.productions() ){
      user |= ranges::action::push_back( discrete.x() );
    }
  }

  ranges::sort( user );

  return user 
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

  for( const auto& [MT, V] : resonances ){
    for( const auto& XS : V ){
      grid |= ranges::action::push_back( XS.x() );
    }
  }
  ranges::sort( grid );

  return grid 
    | ranges::view::unique 
    | ranges::to_vector;
}
