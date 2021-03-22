static
RPVariant collectRP( const Material_t& material ){
  return std::visit( 
    [&]( auto&& arg ){ return Factory::collectRP( arg ); },
    material );
}

static
RPVariant collectRP( const ENDFMaterial_t& material ){
  if( not material.hasFile( 2 ) ){
    Log::error( "ENDF file does not have MF=2." );
    Log::info( "Unable to proceed---until changes are made." );
  }
  return material.file( 2 ).section( 151 ).parse< 2, 151 >();
}
