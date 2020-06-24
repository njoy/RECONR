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

  auto midpoint = []( auto&& x, auto&& y ){
    auto mx =  0.5 * ( std::get<0>(x) + std::get<1>(x) );
    auto my =  0.5 * ( std::get<0>(y) + std::get<1>(y) );
    return std::make_pair( mx, my );
  };
    
  auto eGrid = law.x() | ranges::to_vector;
  auto first = eGrid.begin();
  auto last = eGrid.end();
  std::vector< double > x, y;

  auto cached = law.cachedSearch();
  auto call = [&]( auto&& e ){ return law( e, cached ); };
  auto linearization = twig::linearize::callable( x, y );
  linearization( first, last, call, criterion, midpoint );

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
  using EV = dimwits::Quantity< dimwits::ElectronVolt >;

  auto midpoint = []( auto&& x, auto&& y ){
    auto mx =  0.5 * ( std::get<0>(x) + std::get<1>(x) );
    auto my = std::get<0>(y) + std::get<1>(y);
    my.elastic.value *= 0.5;
    my.fission.value *= 0.5;
    my.capture.value *= 0.5;
    return std::make_pair( mx, my );
  };

  return [=]( auto&& xs ){
    auto criterion = [ & ]( auto&& trial, auto&& reference,
            auto&& xLeft, auto&& xRight,
            auto&&, auto&&  ){

      constexpr double infinity = std::numeric_limits< double >::infinity();

      if ( xRight.value == std::nextafter( xLeft.value, infinity ) ){ 
        return true; }
      auto cDiff = trial - reference;

      double eRelDiff = std::abs( cDiff.elastic/reference.elastic );
      double fRelDiff = std::abs( cDiff.fission/reference.fission );
      double cRelDiff = std::abs( cDiff.capture/reference.capture );

      double diff = std::max( { std::abs( cDiff.elastic.value ), 
                                std::abs( cDiff.fission.value ), 
                                std::abs( cDiff.capture.value ) } );
      double reldiff = std::max( { eRelDiff, fRelDiff, cRelDiff } );

      return ( diff < absTol ) or ( reldiff < relTol );

    };

    auto first = grid.begin();
    auto end = grid.end();
    std::vector< EV > x;
    std::vector< njoy::resonanceReconstruction::breitWigner::CrossSection > y;
    auto linearization = twig::linearize::callable( x, y );
    linearization( first, end, xs, criterion, midpoint );

    return std::make_pair( std::move( x ), std::move( y ) );
  };
}

template< typename Range >
auto
linearize( const Range& grid, 
           resonanceReconstruction::rmatrix::Reconstructor& reconstructor,
           double relTol, double absTol ){
  using EV = resonanceReconstruction::Energy;
  using XS_t = decltype( reconstructor( EV{} ) );

  auto midpoint = []( auto&& x, auto&& y ){
    Log::info( "midpoint of RML" );
    auto mx =  0.5 * ( std::get<0>(x) + std::get<1>(x) );
    return std::make_pair( mx, y );
  };

  auto criterion = [ & ]( auto&& trial, auto&& reference,
          auto&& xLeft, auto&& xRight,
          auto&&, auto&&  ){

    Log::info( "criterion of RML" );
    return true;
  };

  Log::info( "linearizing resonances using an rmatrix::Reconstructor" );

  auto first = grid.begin();
  auto end = grid.end();
  std::vector< EV > x;
  std::vector< XS_t > y;
  auto linearization = twig::linearize::callable( x, y );
  linearization( first, end, reconstructor, criterion, midpoint );

  return 0.0;
}
