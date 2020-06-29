template< typename Range >
static
auto truncateReactions( std::ostream& output,
                        const Range& grid,
                        const std::map< ReactionID, 
                                        Reaction< 
                                          std::vector< double > > >& summed
                        ){
  using Reaction_t = Reaction< std::pair< std::vector< double >,
                                          std::vector< double > > >;
  using RMap_t = std::map< ReactionID, Reaction_t >;
  RMap_t reactions;

  for( const auto& [ ID, reaction ] : summed ){

    auto energies = utility::copy( grid );
    auto xs = reaction.crossSections();
    auto data = std::make_pair( energies, xs );

    reactions.emplace( ID, Reaction_t{ reaction, std::move( data ) } );
  }

  return reactions;
}

template< typename Range >
static
auto truncateReactions( std::ostream& output,
                        const Range& grid,
                        const std::map< ReactionID, 
                                        PPReaction< double > >& summed
                        ){
  using Reaction_t = PPReaction< std::pair< double, double > >;
  using PMap_t = std::map< ReactionID, Reaction_t >;
  PMap_t reactions;

  for( const auto& [ ID, reaction ] : summed ){

    // auto tuples = ranges::view::zip_with( grid, reaction.productions() );
    // auto data = tuples
    //   | ranges::view::transform(
    //     []( auto&& e, auto&& xs ){ return std::make_pair( e, xs ); } )
    //   | ranges::to_vector;

    // reactions.emplace( ID, Reaction_t{ reaction, std::move( data ) } );
  }

  return reactions;
}
