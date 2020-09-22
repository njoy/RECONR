
/*
 * This function will take an ENDF section that behaves like an TAB1.
 *
 * It also checks to see if the first energy point is 
 */
template< typename S >
std::vector< Variant >
TAB1toInterpolation( const S& section, int MT, double threshold = 0.0 ){
  Log::info( "MT: {}", MT );

  std::vector< Variant > cs;

  // We have to convert to a vector to "own" the data
  auto energies = section.energies() | ranges::to_vector;
  auto barns = section.crossSections() | ranges::to_vector;

  // Find duplicate energy points and adjust (i.e., sigfig) to avoid
  // discontinuities
  /*
  auto dupFound = std::adjacent_find( energies.begin(), energies.end() );
  *dupFound = sigfig( *dupFound, -1E-7 );
  auto& next = *( dupFound + 1 );
  next = sigfig( next, +1E-7 );
  */
  /*
  while( dupFound != energies.end() ){
    Log::info( "energies: {}", energies | ranges::view::all );

    auto value = *dupFound;
    *dupFound = sigfig( value, -1E-7 );
    int i = 1;
    while( *( dupFound + i ) == value ){
      *( dupFound + i ) = sigfig( value, +1E-7 );
      i += 1;
    }

    dupFound = std::adjacent_find( dupFound + i, energies.end() );
  }
    */

  auto interpolants = section.interpolants();
  auto boundaries = section.boundaries();
  int left = 0;
  int right = boundaries[ 0 ];
  int drop = 0;
  int take = right;

  /*
  if( threshold != 0.0 ){
    auto energy = energies.begin();
    if( *energy < threshold ){
      auto sigfigged = sigfig( threshold, 1E-7 );
      Log::info( "Changed threshold from {:13.6e} to {:13.6e} for mt {:3}",
                 *energy, sigfigged, MT );
      *energy = sigfigged;

      while( *(energy+1) < *(energy) ){
        *(energy+1) = sigfig( *energy, 1E-7 );
        energy++;
      }
    }
  }
  */
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
