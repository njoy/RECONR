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

template<>
inline
interp::LinearLinear
linearize< interp::LinearLinear >( 
    const interp::LinearLinear& linlin, double, double ){

  return linlin;
}

template< typename Range >
auto
linearizeRes( const Range& grid, double relTol, double absTol ){
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
