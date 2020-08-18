
/*
 * This function will take an ENDF section that behaves like an TAB1
 */
template< typename S >
std::vector< Variant >
TAB1toInterpolation( const S& section ){

  std::vector< Variant > cs;

  // We have to convert to a vector to "own" the data
  auto energies = section.energies() | ranges::to_vector;
  auto barns = section.crossSections() | ranges::to_vector;


  auto interpolants = section.interpolants();
  auto boundaries = section.boundaries();
  int left = 0;
  int right = boundaries[ 0 ];
  int drop = 0;
  int take = right;

  // Do the first one
  auto table = 
    makeInterpolationTable( energies, barns, drop, take, interpolants[ 0 ] );
  cs.emplace_back( std::move( table ) );

  // Do the rest
  for( const auto& [INT, NBT] : 
      ranges::view::zip( interpolants, boundaries ) 
        | ranges::view::drop_exactly( 1 ) ){

    left = right;
    right = NBT;
    take = right - left + 1;
    drop = left - 1;

    auto table = makeInterpolationTable( energies, barns, drop, take, INT );
    cs.emplace_back( std::move( table ) );
  }

  return cs;
}
