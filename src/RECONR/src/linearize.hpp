template< typename LAW >
interp::LinearLinear
linearize2( const LAW & law, double relTol, double absTol ){
  auto criterion = [ & ]( auto&& trial, auto&& reference,
          auto&& xLeft, auto&& xRight,
          auto&&, auto&&  ){

    constexpr double infinity = std::numeric_limits<double>::infinity();

    // Limit of ENDF-6 precision
    auto ratio = 1.0 - ( xLeft/xRight );
    if( ratio < 1E-7 ){ return true; }

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
  auto end = eGrid.end();
  std::vector< double > x, y;

  auto cached = law.cachedSearch();
  auto call = [&]( auto&& e ){ return law( e, cached ); };
  auto linearization = twig::linearize::callable( x, y );
  linearization( first, end, call, criterion, midpoint );

  return interp::LinearLinear{ std::move( x ), std::move( y ) };

}

inline
interp::LinearLinear
linearize( const interp::Histogram& histo, double r, double a ){

  return linearize2( histo, r, a );
  // auto hx = histo.x();
  // auto hy = histo.y();
  // std::vector< double > x{ hx.front() };

  // for( auto& ene : hx | ranges::view::drop_exactly( 1 ) ){
  //   // sigfig( 16, -1 )
  //   x.push_back( sigfig( ene, -1E-7 ) );
  //   x.push_back( ene );
  // }
  // std::sort( x.begin(), x.end() );

  // auto y = x | ranges::view::transform( histo ) | ranges::to_vector;

  // return interp::LinearLinear{ std::move( x ), std::move( y ) };
}

inline
interp::LinearLinear
linearize( const interp::LinearLinear& linlin, double, double ){

  return linlin;
}

inline
interp::LinearLinear
linearize( const interp::LinearLogarithmic& linlog, double r, double a ){

  return linearize2( linlog, r, a );

}

inline
interp::LinearLinear
linearize( const interp::LogarithmicLinear& loglin, double r, double a ){

  return linearize2( loglin, r, a );
}

inline
interp::LinearLinear
linearize( const interp::LogarithmicLogarithmic& loglog, double r, double a ){

  return linearize2( loglog, r, a );
}

/*
template< typename Range >
auto
linearize( const Range& grid, double relTol, double absTol ){
  using EV = dimwits::Quantity< dimwits::ElectronVolt >;

  auto midpoint = []( auto&& energy, auto&& xs ){
    auto midEnergy =  0.5 * ( std::get<0>(energy) + std::get<1>(energy) );
    auto midXS = std::get<0>( xs ) + std::get<1>( xs );
    midXS.elastic *= 0.5;
    midXS.fission *= 0.5;
    midXS.capture *= 0.5;
    return std::make_pair( midEnergy, midXS );
  };

  return [=]( auto&& xs ){
    auto criterion = [ & ]( auto&& trial, auto&& reference,
            auto&& xLeft, auto&& xRight,
            auto&&, auto&&  ){

      constexpr double infinity = std::numeric_limits< double >::infinity();

      if( xRight.value == std::nextafter( xLeft.value, infinity ) ){ 
        return true; }
      // Limit of ENDF-6 precision
      auto ratio = 1.0 - ( xLeft.value/xRight.value );
      if( ratio < 1E-7 ){ return true; }

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
    std::vector< EV > energies;
    std::vector< njoy::resonanceReconstruction::breitWigner::CrossSection > XS;
    auto linearization = twig::linearize::callable( energies, XS );
    linearization( first, end, xs, criterion, midpoint );

    return std::make_pair( std::move( energies ), std::move( XS ) );
  };
}
*/

template< typename Range >
auto
linearize( const Range& grid, 
           resonanceReconstruction::rmatrix::Reconstructor& reconstructor,
           double relTol, double absTol ){
  using EV = resonanceReconstruction::Energy;
  using XS_t = decltype( reconstructor( EV{} ) );

  auto midpoint = []( auto&& energy, auto&& XS ){
    auto midEnergy =  0.5 * ( std::get<0>(energy) + std::get<1>(energy) );

    auto& [ lXS, rXS ] = XS;
    auto IDs = ranges::view::keys( lXS );

    decltype( lXS ) midXS;
    for( const auto& id : IDs ){
      auto y = 0.5*( lXS.at( id ) + rXS.at( id ) );

      midXS.emplace( id, std::move( y ) );
    }
    return std::make_pair( midEnergy, midXS );
  };

  constexpr double infinity = std::numeric_limits< double >::infinity();
  auto criterion = [ & ]( auto&& trial, auto&& reference,
          auto&& xLeft, auto&& xRight,
          auto&&, auto&&  ){


    if( xRight.value == std::nextafter( xLeft.value, infinity ) ){ 
      return true;
    }
    // Limit of ENDF-6 precision
    auto ratio = 1.0 - ( xLeft.value/xRight.value );
    if( ratio < 1E-7 ){ return true; }

    auto IDs = ranges::view::keys( reference );
    for( const auto& id : IDs ){
      auto t = trial.at( id );
      auto r = reference.at( id );

      auto diff = std::abs( t - r );
      auto rdiff = diff/r;

      if( ( diff.value >= absTol ) and ( rdiff >= relTol ) ){ 
        return false;
      }
    }
    return true;
  };

  auto first = grid.begin();
  auto end = grid.end();
  std::vector< EV > energies;
  std::vector< XS_t > crossSections;
  auto linearization = twig::linearize::callable( energies, crossSections );
  linearization( first, end, reconstructor, criterion, midpoint );

  return std::make_pair( std::move( energies ), std::move( crossSections ) );
}
