static
RPVariant collectRP( const Material_t& material ){
  return std::visit( 
    [&]( auto&& arg ){ return Factory::collectRP( arg ); },
    material );
}

static
RPVariant collectRP( const ENDFMaterial_t& material ){
  return material.file( 2 ).section( 151 ).parse< 2, 151 >();
}
