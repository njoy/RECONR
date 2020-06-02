static
XSmap_t collectppXS( const Material_t& material){
  return std::visit( 
    [&](auto&& arg ){ return Factory::collectXS( arg ); },
    material );
}

static
XSmap_t collectppXS( const ENDFMaterial_t& material ){
  XSmap_t xs{};
  auto MF13 = material.fileNumber( 13 ).parse< 13 >();
  return Factory::collectXS( MF13 );
}
