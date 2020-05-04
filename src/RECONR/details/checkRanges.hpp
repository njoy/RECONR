namespace details {

template< typename R1, typename R2 >
void checkRanges( const R1& ref, const R2& trial, std::string name = "" ){
  if( name != ""){
    njoy::Log::info( "name: {}", name );
    njoy::Log::info( "ref: {}", ref | ranges::view::all );
    njoy::Log::info( "tri: {}", trial | ranges::view::all );
  }
  REQUIRE( ranges::distance( ref ) == ranges::distance( trial ) );
  for( const auto& [r,t] : ranges::view::zip(ref, trial) ){
    CHECK( r == Approx( t ).epsilon( 5E-6 ) );
  }
}

} // namespace details 
