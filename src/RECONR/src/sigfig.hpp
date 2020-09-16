/* sigfig will nudge a value up (or down) a given amount. 
 *
 * If the parameter, nudge, is positive, sigfig will nudge up.
 * If the parameter, nudge, is negative, sigfig will nudge down.
 *
 * @param value: The value to be nudged.
 * @param nudge: The relative fraction of value by which value will be nudged.
 *
 * I didn't want to write this, but it is just needed in so many places to be
 * conformant with Legacy NJOY
 */
template< typename V >
constexpr V sigfig( const V& value, const double nudge = 1E-8 ){
  return value*( 1 + nudge );
}
