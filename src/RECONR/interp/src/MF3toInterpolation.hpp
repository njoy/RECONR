
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

  auto makeInterpTable = [&]( int drop, int take, auto& LAW ){
    auto pE = partition( energies, drop,  take );
    auto pB = partition( barns, drop,  take );

    switch( LAW ){
      case 1: {
        cs.emplace_back( Variant( 
            Histogram( std::move( pE ), std::move( pB ) ) ) );
        break;
      }
      case 2: {
        cs.emplace_back( Variant( 
          LinearLinear( std::move( pE ), std::move( pB ) ) ) );
        break;
      }
      case 3: {
        cs.emplace_back( Variant( 
          LinearLogarithmic( std::move( pE ), std::move( pB ) ) ) );
        break;
      }
      case 4: {
        cs.emplace_back( Variant( 
          LogarithmicLinear( std::move( pE ), std::move( pB ) ) ) );
        break;
      }
      case 5: {
        cs.emplace_back( Variant( 
          LogarithmicLogarithmic( 
              std::move( pE ), std::move( pB ) ) ) );
        break;
      }
      default: {
        Log::error( "Invalid interpolation parameter." );
        throw std::exception();
      }
    } // switch( LAW )
  }; // makeInterpTable

  auto interpolants = section.interpolants();
  auto boundaries = section.boundaries();
  int left = 0;
  int right = boundaries[ 0 ];
  int drop = 0;
  int take = right;

  // Do the first one
  makeInterpTable( drop, take, interpolants[ 0 ] );

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
    makeInterpTable( drop, take, std::get< 0 >( params ) );
  }

  return cs;
}
