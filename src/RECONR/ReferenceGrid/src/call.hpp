std::vector< double > operator()( const RP::SpecialCase& sc ){
  return { sc.EL(), sc.EH() };
}

std::array<double, 3> operator()( 
  const RP::resolved::BreitWigner::Lvalue::Resonance& resonance ){

  const double E = resonance.ER();
  const double hw = resonance.GT()*0.5;
  return {{E-hw, E, E+hw}};
}

std::array<double, 3> operator()( 
  const RP::resolved::ReichMoore::Lvalue::Resonance& resonance ){

  const double E = resonance.ER();
  const double hw = 0.5*(resonance.GN() + 
                         resonance.GG() +
                         resonance.GFA() +
                         resonance.GFB() );
  return {{E-hw, E, E+hw}};
}

template< typename BWL, 
          typename =decltype( std::declval< BWL >().resonances() ) >
auto operator()( const BWL& lValues ){
  return lValues
    | ranges::view::for_each( []( auto&& L ){ return L.resonances(); } )
    | ranges::view::for_each( *this );
}

// template< typename URR, 
//           typename =decltype( std::declval< URR >().jValues() ) >
// auto operator()( const URR& lValues ){
// }

template< typename T >
auto operator()( const T& range ){
  std::vector<double> energies = range.lValues() 
    | ranges::view::transform( *this )
    | ranges::view::drop_while(
        [&]( const auto& E ){ return E < range.EL(); } )
    | ranges::view::take_while(
        [&]( const auto& E ){ return E < range.EH(); } )
    | ranges::to_vector;

  energies.push_back( range.EL() );
  energies.push_back( range.EH() );

  return energies;
}

// template< typename... TS >
// auto operator()( const std::variant< TS... >& ts ){
//   return std::visit( *this, ts );
// }

// auto operator()( const RP::Isotope& isotope ){
//   return ranges::action::join( 
//     isotope.energyRanges() | ranges::view::transform( *this ) );
// }
