static void fill( const double lowerBound,
                  const double upperBound,
                  std::vector<double>& buffer ){
  constexpr double thresholdRatio = 1.193776641714434; // 10^(1/13)
  auto epsilon = std::nextafter( 
    0.0, std::numeric_limits< double >::infinity() );
  if ( lowerBound * thresholdRatio < upperBound ){
    buffer |=
      ranges::action::push_back( 
        ranges::view::generate( 
          [ =, state = lowerBound ]() mutable { 
            // We have to add epsilon in case state == 0
            state = state*thresholdRatio + epsilon;
            return state; }
        )
        | ranges::view::take_while ( 
            [ = ]( auto energy ){ return energy < upperBound; } 
          ) 
        );
  } else {
    buffer.push_back( upperBound );
  }
}
