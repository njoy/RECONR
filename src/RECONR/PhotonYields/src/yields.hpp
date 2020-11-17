template< typename F >
auto yields() const{
  try {
    return this->yields_
      | ranges::view::transform(
        []( auto&& y ){ return std::get< F >( y ); }
      );
  } catch( std::bad_variant_access& ){
    Log::error( "Error asking production information of PhotonYields." );
    Log::info( "Asked for variant member which didn't exist" );
    throw;
  }
}

void yields( std::vector< PYForms >&& yields ){ 
  this->yields_ = std::move( yields );
}
