template< typename F >
const auto
productions() const{
  try {
    return this->productions_
      | ranges::view::transform(
        []( auto&& p ){ return std::get< F >( p ); }
      );
  } catch( std::bad_variant_access& ){
    Log::error( "Error asking production information of PhotonProduction." );
    Log::info( "Asked for variant member which didn't exist" );
    throw;
  }
}

void productions( std::vector< PPForms >&& prods ){ 
  this->productions_ = std::move( prods );
}

// const std::vector< XS >& productions() const{ return this->productions_; }

