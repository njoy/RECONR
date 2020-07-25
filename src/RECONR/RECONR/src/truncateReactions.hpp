template< typename X, typename Y >
static
std::pair< std::vector< double >, 
           std::vector< double > >
truncate( X&& x, Y&& y ){
  auto isZero = []( auto&& cs ){ return cs == 0.0; };
  auto first = std::find_if_not( y.begin(), y.end(), isZero );

  // decltype( std::distance( y.begin(), first ) ) start;
  auto start{ std::distance( y.begin(), first ) };
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

static
void truncateReactions( std::ostream& output, R2D2& r2d2 ){

  output << "\nTruncating leading zeros of reconstructed reactions." 
         << std::endl;

  auto trunc = [&]( auto&& reactions ){
    for( auto& [ ID, reaction ] : reactions ){
      output << fmt::format( "\n\t{:20s} ", ID.symbol() );

      auto xs = reaction.template crossSections< XSPair >();

      auto data = truncate( xs.first, xs.second );
      if( data.first.size() > 0 ){
        reaction.crossSections( std::move( data ) );
      } else{
        continue;
      }
    }
  };

  trunc( r2d2.reactions() );
  trunc( r2d2.summations() );
}
