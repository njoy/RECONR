template< typename LAW >
interp::LinearLinear
linearize2( LAW law, double relTol, double absTol ){
  Log::info( "Generic LAW" );
  auto criterion = [ & ]( auto&& trial, auto&& reference,
          auto&& xLeft, auto&& xRight,
          auto&&, auto&&  ){
          // auto&& yLeft, auto&& yRight ){

    constexpr double infinity = std::numeric_limits<double>::infinity();

    if ( xRight == std::nextafter( xLeft, infinity ) ){ 
      return true; 
    }
    auto diff = std::abs( trial - reference );
    auto reldiff = (diff/reference);

    // Log::info( R"(
// criterion:
// trial: {:.16E}, refer: {:.16E}
// xleft: {:.16E}, xRight: {:.16E} 
// yleft: {:.16E}, yRight: {:.16E} 
// diffe: {:.16E}, reldif: {:.16E}
  // )", trial, reference, xLeft, xRight, yLeft, yRight, diff, reldiff );
    return ( diff < absTol ) or ( reldiff < relTol );
  };

  auto midpoint = []( auto&& x, auto&& ){
    return 0.5 * ( std::get<0>(x) + std::get<1>(x) );
  };
    
  auto eGrid = law.x() | ranges::to_vector;
  auto first = eGrid.begin();
  auto last = eGrid.end();
  std::vector< double > x;
  auto linearization = twig::linearize::callable< double >( x );
  linearization( first, last, law, criterion, midpoint );

  auto y = x | ranges::view::transform( law ) | ranges::to_vector;

  return interp::LinearLinear{ std::move( x ), std::move( y ) };

}

inline
interp::LinearLinear
linearize( interp::Histogram histo, double, double ){
  Log::info( "Histogram LAW" );

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
  Log::info( "LinearLinear LAW" );

  return linlin;
}

inline
interp::LinearLinear
linearize( interp::LinearLogarithmic linlog, double r, double a ){
  Log::info( "LinearLogarithmic LAW" );

  return linearize2( linlog, r, a );

}

inline
interp::LinearLinear
linearize( interp::LogarithmicLinear loglin, double r, double a ){
  Log::info( "LogarithmicLinear LAW" );

  return linearize2( loglin, r, a );
}

inline
interp::LinearLinear
linearize( interp::LogarithmicLogarithmic loglog, double r, double a ){
  Log::info( "LogarithmicLogarithmic LAW" );

  return linearize2( loglog, r, a );
}
