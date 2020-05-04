namespace details{

template < typename T >
T nextMin( T value ){
  return std::nextafter( value, std::numeric_limits< T >::min() ); 
}

} // namespace details
