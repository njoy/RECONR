static
Info_t information( const Material_t& material ){
  return std::visit(
    []( auto&& mat ){ return Factory::information( mat ); },
    material
  );
}

static
Info_t information( const ENDFMaterial_t& material ){
  return material.fileNumber( 1 ).sectionNumber( 451 ).parse< 1, 451 >();
}
