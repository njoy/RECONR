/* sigfig will nudge a value up (or down) a given amount. 
 *
 * If the parameter, nudge, is positive, sigfig will nudge up.
 * If the parameter, nudge, is negative, sigfig will nudge down.
 *
 * @param sigdig: The value to be nudged.
 * @param magnitude: The relative fraction of value by which value will be nudged.
 *
 * I didn't want to write this, but it is just needed in so many places to be
 * conformant with Legacy NJOY
 */

template< typename V >
double sigfig( const V& value, const int sigdig, const int magnitude = 1){

  if( sigdig < 0 ){
    Log::error( 
      "The number of significant digits (sigdig) in sigif must be >= 0." );
    Log::info( "sigdig = {}", sigdig );
    throw std::exception();
  }

  if( value == 0.0 ){
    return magnitude * std::copysign( std::pow( 10, -sigdig ), magnitude );
  }

  int power = sigdig - 1 - std::floor( std::log10( std::abs( value ) ) );

  auto nudge = std::pow( 10, sigdig - 11 - power ) +
               magnitude*std::pow( 10, -power );

  return value + nudge;

}
