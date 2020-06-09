static
XSMap_t collectXS( const Material_t& material){
  return std::visit( 
    [&](auto&& arg ){ return Factory::collectXS( arg ); },
    material );
}

static
XSMap_t collectXS( const ENDFMaterial_t& material ){
  XSMap_t xs{};

  auto MF3 = material.fileNumber( 3 ).parse< 3 >();
  for( auto& section : MF3.sections() ){
    Reaction< RawInterpolation_t > reaction{ 
      section.ZA(),
      section.AWR(),
      section.QM(),
      section.QI(),
      section.LR(),
      interp::MF3toInterpolation( section ) };

    xs.insert( std::make_pair( section.MT(), std::move( reaction ) ) );
  }

  return xs;
}
