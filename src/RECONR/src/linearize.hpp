
template< typename LAW >
interp::LinearLinear
linearize( const LAW & law, double relTol, double absTol ){
  auto criterion = [ & ]( auto&& trial, auto&& reference,
          auto&& xLeft, auto&& xRight,
          auto&&, auto&&  ){

    if( linearizeCriterion( xLeft, xRight ) ){ return true; }
    else{ return linearizeCriterion(trial, reference, relTol, absTol); }
  };

  auto midpoint = []( auto&& x, auto&& y ){
    auto& [x0, x1] = x;
    auto mx =  0.5 * ( x0 + x1 );
    mx = utility::sigfig( mx, 9, 0 );

    auto& [y0, y1] = y;
    auto my = interpolation::LinearLinear::apply(mx, x0, x1, y0, y1 );
    my = utility::sigfig( my, 9, 0 );
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

template<>
inline
interp::LinearLinear
linearize< interp::LinearLinear >( const interp::LinearLinear& linlin, 
                                   double, double ){
  return linlin;
}

template< typename Range >
auto
linearizeRes( const Range& grid, double relTol, double absTol ){
  using EV = dimwits::Quantity< dimwits::ElectronVolt >;

  auto midpoint = []( auto&& energy, auto&& xs ){
    decltype( auto ) x0 = std::get< 0 >( energy );
    decltype( auto ) x1 = std::get< 1 >( energy );
    auto midEnergy =  0.5 * ( x0 + x1 );
    midEnergy.value = utility::sigfig( midEnergy.value, 9, 0 );

    auto interp = [&]( auto&& y0, auto&& y1 ){
      return interpolation::LinearLinear::apply( 
          midEnergy.value, x0.value, x1.value, y0, y1 );
    };


    auto& [y0, y1] = xs;
    auto midXS = y0;
    midXS.elastic = utility::sigfig( interp( y0.elastic, y1.elastic ), 9, 0 );
    midXS.fission = utility::sigfig( interp( y0.fission, y1.fission ), 9, 0 );
    midXS.capture = utility::sigfig( interp( y0.capture, y1.capture ), 9, 0 );
    return std::make_pair( midEnergy, midXS );
  };

  return [=]( auto&& xs ){
    auto criterion = [ & ]( auto&& trial, auto&& reference,
            auto&& xLeft, auto&& xRight,
            auto&&, auto&&  ){

      if( linearizeCriterion( xLeft.value, xRight.value ) ){ return true; }

      if( not linearizeCriterion( 
                trial.elastic.value, reference.elastic.value,
                relTol, absTol ) ){
        return false;
      }
      else if( not linearizeCriterion( 
                trial.fission.value, reference.fission.value,
                relTol, absTol ) ){
        return false;
      }
      else if( not linearizeCriterion( 
                trial.capture.value, reference.capture.value,
                relTol, absTol ) ){
        return false;
      }
      else{
        return true;
      }
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

template< typename Range >
auto
linearize( const Range& grid, 
           resonanceReconstruction::rmatrix::Reconstructor& reconstructor,
           double relTol, double absTol ){
  using EV = resonanceReconstruction::Energy;
  using XS_t = decltype( reconstructor( EV{} ) );

  auto midpoint = []( auto&& energy, auto&& XS ){
    decltype( auto ) x0 = std::get< 0 >( energy );
    decltype( auto ) x1 = std::get< 1 >( energy );
    auto midEnergy =  0.5 * ( x0 + x1 );
    midEnergy.value = utility::sigfig( midEnergy.value, 9, 0 );

    auto interp = [&]( auto&& y0, auto&& y1 ){
      return interpolation::LinearLinear::apply( 
          midEnergy.value, x0.value, x1.value, y0, y1 );
    };

    auto& [ lXS, rXS ] = XS;
    decltype( lXS ) midXS;

    auto IDs = ranges::view::keys( lXS );
    for( const auto& id : IDs ){
      auto y = lXS.at( id );
      y.value = utility::sigfig( interp( lXS.at( id ).value, 
                                         rXS.at( id ).value ), 9, 0 );

      midXS.emplace( id, std::move( y ) );
    }
    return std::make_pair( midEnergy, midXS );
  };

  auto criterion = [ & ]( auto&& trial, auto&& reference,
          auto&& xLeft, auto&& xRight,
          auto&&, auto&&  ){

    if( linearizeCriterion( xLeft.value, xRight.value ) ){ return true; }

    auto IDs = ranges::view::keys( reference );
    for( const auto& id : IDs ){
      auto t = trial.at( id );
      auto r = reference.at( id );

      if( not linearizeCriterion( t.value, r.value, relTol, absTol ) ){
        return false;
      }
    }
    // If we haven't returned false yet, we are true
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
