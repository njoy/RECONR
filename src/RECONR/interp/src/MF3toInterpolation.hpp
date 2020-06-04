
/*
 * This function will take an MF3 section and return an interpolation table.
 */
inline
std::vector< Variant >
MF3toInterpolation( const ENDFtk::section::Type< 3 >& section ){

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
  cs.emplace_back(
    makeInterpolationTable( energies, barns, drop, take, interpolants[ 0 ] )
  );

  // Do the rest
  for( const auto& params : 
      ranges::view::zip( interpolants, boundaries ) 
        | ranges::view::drop_exactly( 1 ) ){

    left = right;
    right = std::get< 1 >( params );
    take = right - left;
    drop = left;

    // interpolation needs to have double points
    if( energies[ left ] != energies[ left + 1] ){
      drop -= 1;
      take +=1;
    }
    cs.emplace_back(
      makeInterpolationTable( 
          energies, barns, drop, take, std::get< 0 >( params ) )
    );
  }

  return cs;
}
