template< typename X, typename Y >
static
std::pair< std::vector< double >, 
           std::vector< double > >
truncateReactions( X&& x, Y&& y ){
  auto isZero = []( auto&& cs ){ return cs == 0.0; };
  auto first = std::find_if_not( y.begin(), y.end(), isZero );

  decltype( std::distance( y.begin(), first ) ) start;
  if( first == y.begin() ){
    start = 0;
  } else{
    start = std::distance( y.begin(), first ) - 1;
  }

  auto data = std::make_pair( 
    std::vector< double >( x.begin() + start, x.end() ),
    std::vector< double >( y.begin() + start, y.end() )
  );

  return data;
}

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

    auto xs = reaction.crossSections();

    auto data = truncateReactions( grid, xs );
    if( data.first.size() > 0 ){
      reactions.emplace( ID, Reaction_t{ reaction, std::move( data ) } );
    } else{
      continue;
    }
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

    Log::info( "ID: {}", ID );
    auto prod = reaction.productions();
    Log::info( "prod: {}", prod | ranges::view::all );
    auto data = truncateReactions( grid, prod );
    Log::info( "data.first: {}", data.first | ranges::view::all );
    Log::info( "data.second: {}", data.second | ranges::view::all );

    // auto tuples = ranges::view::zip_with( data.first, data.second )
    //   | ranges::to_vector;
    // reactions.emplace( ID, Reaction_t{ reaction, std::move( tuples ) } );
  }

  return reactions;
}
