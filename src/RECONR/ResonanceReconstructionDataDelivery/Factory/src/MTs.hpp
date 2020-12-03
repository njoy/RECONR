static
auto MTs( const ENDFMaterial_t& material ){
  return material.file( 3 ).sectionNumbers();
}

static
auto MTs( const Material_t& material ){
  return std::visit(
    []( auto&& mat ){ return Factory::MTs( mat ); },
    material
  );
}
