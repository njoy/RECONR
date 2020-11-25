/* sigfig will nudge a value up (or down) a given amount. 
 *
 * If the parameter, nudge, is positive, sigfig will nudge up.
 * If the parameter, nudge, is negative, sigfig will nudge down.
 *
 * @param value: The value to be nudged.
 * @param sigdig: The number of signficant digits to keep.
 * @param magnitude: The relative fraction of value by which value will be nudged.
 *
 * I didn't want to write this, but it is just needed in so many places to be
 * conformant with Legacy NJOY
 */

template< typename V >
double sigfig( const V& value, const int sigdig, const int magnitude = 1){

  if( value == 0.0 ){
    return magnitude * std::copysign( std::pow( 10, -sigdig ), magnitude );
  }

  int p = std::floor( std::log10( std::fabs( value ) ) );
  double mult = std::pow( 10, sigdig - p - 1 );
  return std::round(value*mult + magnitude)/mult;
}

template< typename Q >
dimwits::Quantity< Q >
sigfig( const dimwits::Quantity< Q >& v, const int sigdig, const int magnitude = 1 ){
  dimwits::Quantity< Q > q;
  q.value = sigfig( v.value, sigdig, magnitude );
  return q;
}
