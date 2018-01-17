static void fill( const double lowerBound,
                  const double upperBound,
                  std::vector<double>& buffer ){
  constexpr double thresholdRatio = 1.193776641714434; // 10^(1/13)
  if ( lowerBound * thresholdRatio < upperBound ){
    buffer |=
      ranges::action::push_back
      ( ranges::view::generate
        ( [ =, state = lowerBound ]() mutable { return state *= thresholdRatio; } )
        | ranges::view::take_while
          ( [ = ]( auto energy ){ return energy < upperBound; } ) );
    buffer.push_back( upperBound );
  }
}
