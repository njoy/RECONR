std::array<double, 3>
operator()( const resolved::BreitWigner::Lvalue::Resonance& resonance ) const {
  const double energy = resonance.ER();
  const double halfWidth = 0.5 * resonance.GT();
  return {{ energy - halfWidth, energy, energy + halfWidth }};
}

std::array<double, 3>
operator()( const resolved::ReichMoore::Lvalue::Resonance& resonance ) const {
  const double energy = resonance.ER();
  const double halfWidth = 0.5 * ( resonance.GN()
                                   + resonance.GG()
                                   + resonance.GFA()
                                   + resonance.GFB() );
  return {{ energy - halfWidth, energy, energy + halfWidth }};
}

template< typename ResolvedLvalue,
          if_< hasResonances< ResolvedLvalue > > = true >
auto
operator()( const ResolvedLvalue& lValue ) const {
  return lValue.resonances() | ranges::view::transform( *this );
}

template< typename Range, if_< hasLvalues< Range > > = true >
std::vector<double>
operator()( const Range& range ) const {
  std::vector< double > energies;
  energies.reserve( 12 * range.lValues().size() );

  const auto lowerLimit = range.EL();
  const auto upperLimit =
    std::nextafter( range.EH(), -std::numeric_limits< double >::infinity() );

  energies.push_back( lowerLimit );

  energies |=
    ranges::action::push_back( range.lValues()
                               | ranges::view::for_each( *this )
                               | ranges::view::filter
                                 ( [=]( const auto energy )
                                   { return energy <= lowerLimit
                                            or energy >= upperLimit; } ) );

  energies.push_back( upperLimit );

  if ( not ranges::is_sorted( energies ) ){
    orpl::pdqsort( energies.begin(), energies.end() );
  }

  energies |= ranges::action::unique;
  return energies;
}

std::vector< double > operator()( const unresolved::CaseA& caseA ) const {
  std::vector< double > energies;
  const auto lowerLimit = caseA.EL();
  const auto upperLimit =
    std::nextafter( caseA.EH(), -std::numeric_limits< double >::infinity() );
  energies.reserve( std::ceil( 5.645828264742274 *
                               std::log( upperLimit / lowerLimit ) ) );
  energies.push_back( lowerLimit );
  fill( lowerLimit, upperLimit, energies );
  return energies;
}

std::vector< double > operator()( const unresolved::CaseB& caseB ) const {
  std::vector< double > energies;
  const auto lowerLimit = range.EL();
  const auto upperLimit =
    std::nextafter( range.EH(), -std::numeric_limits< double >::infinity() );
  energies.reserve( 8 * std::ceil( std::log( upperLimit / lowerLimit ) ) );
  energies.push_back( lowerLimit );
  const auto clampedES =
    caseB.ES()
    | ranges::view::drop_while
      ( [=]( auto energy ){ energy <= lowerLimit; } )
    | ranges::view::take_while
      ( [=]( auto energy ){ energy < upperLimit; } );

  for( const auto energy : clampedES ){
    fill( energies.back(), energy, energies );
  }

  fill( energies.back(), upperLimit, energies );
}

template< typename UnresolvedLvalue,
          if_< not hasResonances< UnresolvedLvalue > > = true >
auto
operator()( const UnresolvedLvalue& lValue ) const {
  return
    lValue.jValues()
    | range::view::transform
      ( []( const auto& jValue ){ return jValue.ES(); } );
}

std::vector< double > operator()( const unresolved::CaseC& caseC ) const {
  std::vector< double > energies;

  const auto lowerLimit = caseC.EL();
  const auto upperLimit =
    std::nextafter( caseC.EH(), -std::numeric_limits< double >::infinity() );

  std::vector< double > firstPass;
  firstPass.push_back( lowerLimit );

  firstPass |=
    ranges::action::push_back( range.lValues()
                               | ranges::view::for_each( *this )
                               | ranges::view::filter
                                 ( [=]( const auto energy )
                                   { return energy <= lowerLimit
                                            or energy >= upperLimit; } ) );

  firstPass.push_back( upperLimit );
  orpl::pdqsort( energies.begin(), energies.end() );
  firstPass |= ranges::action::unique;

  energies.reserve( 8 * firstPass.size() );
  energies.push_back( firstPass.front() );

  for( const auto energy : firstPass | ranges::view::drop_exactly(1) ){
    fill( energies.back(), energy, energies );
  }

  return energies;
}

std::vector< double > operator()( const SpecialCase& sc ) const {
  return { sc.EL(), sc.EH() };
}

template< typename... TS >
auto operator()( const std::variant< TS... >& range_variant ) const {
  return std::visit( *this, range_variant );
}

/*
auto operator()( const RP::Isotope& isotope ) const {
  return ranges::action::join(
    isotope.energyRanges() | ranges::view::transform( *this ) );
}
*/
