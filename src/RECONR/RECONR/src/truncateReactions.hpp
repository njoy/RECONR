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
void truncateReactions( std::ostream& output,
                        R2D2::XSMap_t& summed
                        ){

  for( auto& [ ID, reaction ] : summed ){

    auto xs = reaction.crossSections< XSPair >();

    auto data = truncate( xs.first, xs.second );
    if( data.first.size() > 0 ){
      reaction.crossSections( std::move( data ) );
    } else{
      continue;
    }
  }
}

static
void truncateReactions( std::ostream& output,
                        R2D2::PPMap_t& summed
                        ){
  for( auto& [ ID, production ] : summed ){

    Log::info( "ID: {}", ID );
    auto prod = production.template productions< PPair >();
    auto energies = prod
      | ranges::view::transform( []( auto&& p ){ return p.first; } );
    auto productions = prod
      | ranges::view::transform( []( auto&& p ){ return p.second; } );

    auto data = truncate( energies, productions );

    auto tuples = ranges::view::zip_with( 
      []( auto&& e, auto&& p ){ return PPForms{ std::make_pair( e, p ) }; },
      data.first, data.second )
      | ranges::to_vector;

    production.productions( std::move( tuples ) );
  }
}

static
void truncateReactions( std::ostream& output,
                        R2D2& data ){
  truncateReactions( output, data.reactions() );
  truncateReactions( output, data.photonProductions() );

}
