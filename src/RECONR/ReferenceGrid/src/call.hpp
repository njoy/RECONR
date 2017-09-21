std::vector< double > operator()( const RP::SpecialCase& sc ) const {
  return { sc.EL(), sc.EH() };
}

std::array<double, 3> operator()( 
  const RP::resolved::BreitWigner::Lvalue::Resonance& resonance ) const {

  const double E = resonance.ER();
  const double hw = resonance.GT()*0.5;
  return {{E-hw, E, E+hw}};
}

auto operator()( 
  const RP::resolved::ReichMoore::Lvalue::Resonance& resonance ) const {

  const double E = resonance.ER();
  const double hw = 0.5*(resonance.GN() + 
                         resonance.GG() +
                         resonance.GFA() +
                         resonance.GFB() );
  buffer = {{E-hw, E, E+hw}};
  return ranges::make_iterator_range( buffer.begin(), buffer.end() );
}

template< typename resolvedL >
auto operator()( const resolvedL& lValue,
                 std::conditional_t< true, void*,  decltype( lValue.resonances() ) > = nullptr) const {

  return lValue.resonances()
    | ranges::view::for_each( *this );
}

// template< typename URR, 
//           typename =decltype( std::declval< URR >().jValues() ) >
// auto operator()( const URR& lValues ) const {
// }

template< typename T >
auto operator()( const T& range,
                 std::conditional_t< true, void*,  decltype( std::declval< T >().lValues() ) > = nullptr) const {
  auto energies = range.lValues()
    | ranges::view::for_each( *this )
    // | ranges::view::drop_while(
    //     [&]( const auto& E ){ return E < range.EL(); } );
    // | ranges::view::take_while(
    //     [&]( const auto& E ){ return E < range.EH(); } )
    | ranges::to_vector;
  // echo< decltype( e0 ) > {};

  // energies.push_back( range.EL() );
  // energies.push_back( range.EH() );

  return energies;
}

// template< typename... TS >
// auto operator()( const std::variant< TS... >& ts ) const {
//   return std::visit( *this, ts );
// }

// auto operator()( const RP::Isotope& isotope ) const {
//   return ranges::action::join( 
//     isotope.energyRanges() | ranges::view::transform( *this ) );
// }
//
