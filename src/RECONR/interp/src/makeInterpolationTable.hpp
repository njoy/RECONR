template< typename E, typename B, typename L >
inline
Variant makeInterpolationTable( const E& energies, const B& barns,
                                int drop, int take, L& LAW ){
  auto pE = partition( energies, drop,  take );
  auto pB = partition( barns, drop,  take );

  switch( LAW ){
    case 1: {
      auto s = ranges::distance( pE );
      for( int i=1; i < s-1; i++ ){
        auto& right = pE[ i ];

        if( pE[ i-1 ] == right ){ 
          right = utility::sigfig( right, 9, +1 );
        }
      }
      if( pE[ s-2 ] == pE.back() ){ 
        pE[ s-2 ] = utility::sigfig( pE[ s-2 ], 9, -1 );
      }
      return Variant( HogHog( std::move( pE ), std::move( pB ) ) );
      break;
    }
    case 2: {
      return Variant( LinLin( std::move( pE ), std::move( pB ) ) );
      break;
    }
    case 3: {
      return Variant( LinLog( std::move( pE ), std::move( pB ) ) );
      break;
    }
    case 4: {
      return Variant( LogLin( std::move( pE ), std::move( pB ) ) );
      break;
    }
    case 5: {
      return Variant( LogLog( std::move( pE ), std::move( pB ) ) );
      break;
    }
    default: {
      Log::error( "Invalid interpolation parameter." );
      throw std::exception();
    }
  } // switch( LAW )
}
