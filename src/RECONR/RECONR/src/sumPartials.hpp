template< typename Range >
static
std::vector< double > sumPartials( const Range& partials ){

  auto sumTuple = []( auto&& tuple ){ 
    return std::apply(
      [](auto... v) { return (v + ...); },
      tuple 
    );
  };

  return ranges::accumulate( partials,
    std::vector< double >( partials[0].size(), 0.0 ),
    [&]( const auto& acc, const auto& next ){
      return ranges::view::zip( acc, next )
        | ranges::view::transform( sumTuple )
        | ranges::to_vector;
    }
  );
}
