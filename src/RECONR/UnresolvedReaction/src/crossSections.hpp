template< typename F >
const auto& 
crossSections() const{ 
  try {
    return std::get< F >( this->crossSection_ );
  } catch( std::bad_variant_access& ){
    Log::error( 
      "Error accessing cross section information of UnresolvedReaction." );
    Log::info( 
      fmt::format( "Asked for variant index: {}.", crossSection_.index() ) );
    Log::info( 
      fmt::format( "Currently holding index: {}.", crossSection_.index() ) );
    throw;
  }
}

void crossSections( UForms&& xs ){ this->crossSection_ = std::move( xs ); }
