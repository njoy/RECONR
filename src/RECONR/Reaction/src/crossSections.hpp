template< typename F >
const auto& 
crossSections() const{ 
  try {
    return std::get< F >( this->crossSection_ );
  } catch( std::bad_variant_access& ){
    Log::error( "Error accessing cross section information of Reaction." );
    Log::info( "Asked for variant member which doesn't exist." );
    throw;
  }
}

void crossSections( XSForms&& xs ){ this->crossSection_ = std::move( xs ); }
