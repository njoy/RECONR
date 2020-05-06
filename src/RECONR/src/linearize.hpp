template< typename LAW >
interp::LinearLinear
linearize2( LAW law, double relTol, double absTol ){
  auto criterion = [ & ]( auto&& trial, auto&& reference,
          auto&& xLeft, auto&& xRight,
          auto&&, auto&&  ){

    constexpr double infinity = std::numeric_limits<double>::infinity();

    if ( xRight == std::nextafter( xLeft, infinity ) ){ return true; }
    auto diff = std::abs( trial - reference );
    auto reldiff = (diff/reference);

    return ( diff < absTol ) or ( reldiff < relTol );
  };

  auto midpoint = []( auto&& x, auto&& ){
    return 0.5 * ( std::get<0>(x) + std::get<1>(x) );
  };
    
  auto eGrid = law.x() | ranges::to_vector;
  auto first = eGrid.begin();
  auto last = eGrid.end();
  std::vector< double > x, y;
  auto linearization = twig::linearize::callable( x, y );
  linearization( first, last, law, criterion, midpoint );

  return interp::LinearLinear{ std::move( x ), std::move( y ) };

}

inline
interp::LinearLinear
linearize( interp::Histogram histo, double, double ){

  auto hx = histo.x();
  auto hy = histo.y();
  std::vector< double > x{ hx.front() };

  for( auto& ene : hx | ranges::view::drop_exactly( 1 ) ){
    // sigfig( 16, -1 )
    x.push_back( 
      std::nextafter( ene, std::numeric_limits< double >::min( ) ) );
    x.push_back( ene );
  }

  auto y = x | ranges::view::transform( histo ) | ranges::to_vector;

  return interp::LinearLinear{ std::move( x ), std::move( y ) };
}

inline
interp::LinearLinear
linearize( interp::LinearLinear linlin, double, double ){

  return linlin;
}

inline
interp::LinearLinear
linearize( interp::LinearLogarithmic linlog, double r, double a ){

  return linearize2( linlog, r, a );

}

inline
interp::LinearLinear
linearize( interp::LogarithmicLinear loglin, double r, double a ){

  return linearize2( loglin, r, a );
}

inline
interp::LinearLinear
linearize( interp::LogarithmicLogarithmic loglog, double r, double a ){

  return linearize2( loglog, r, a );
}

template< typename Range >
auto
linearize( const Range& grid, double relTol, double absTol ){

  auto midpoint = []( auto&& x, auto&& ){
    return 0.5 * ( std::get<0>(x) + std::get<1>(x) );
  };
    
  auto criterion = [ & ]( auto&& trial, auto&& ref,
          auto&& xLeft, auto&& xRight,
          auto&&, auto&&  ){

    constexpr double infinity = std::numeric_limits<double>::infinity();

    if ( xRight == std::nextafter( xLeft, infinity ) ){ return true; }
    auto eDiff = std::abs( trial.elastic.value - ref.elastic.value );
    auto fDiff = std::abs( trial.fission.value - ref.fission.value );
    auto cDiff = std::abs( trial.capture.value - ref.capture.value );
    auto eRelDiff = eDiff/ref.elastic.value;
    auto fRelDiff = fDiff/ref.fission.value;
    auto cRelDiff = cDiff/ref.capture.value;

    auto diff = std::max( eDiff, std::max( fDiff, cDiff ) );
    auto reldiff = std::max( eRelDiff, std::max( fRelDiff, cRelDiff ) );

    return ( diff < absTol ) or ( reldiff < relTol );
  };

  return [&]( auto&& xs ){

    Log::info( "grid: " );
    for( auto& g : grid ){
      Log::info( "\t{}", g );
    }
    auto first = grid.begin();
    auto end = grid.end();
    std::vector< double > x, y;
    auto linearization = twig::linearize::callable( x, y );
    linearization( first, end, xs, criterion, midpoint );

    return interp::LinearLinear{ std::move( x ), std::move( y ) };
  };
}
