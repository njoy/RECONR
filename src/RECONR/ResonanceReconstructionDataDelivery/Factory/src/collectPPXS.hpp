static
PPMap_t collectPPXS( const Material_t& material){
  return std::visit( 
    [&](auto&& arg ){ return Factory::collectPPXS( arg ); },
    material );
}

static
PPMap_t collectPPXS( const ENDFMaterial_t& material ){
  PPMap_t xs{};

  if( material.hasFileNumber( 13 ) ){
    Log::info( "Extracting from MF=13" );
    auto MF13 = material.fileNumber( 13 ).parse< 13 >();
    for( const auto& section : MF13.sections() ){
      PPReaction reaction{
        section.ZA(), section.AWR(),
        interp::MF13toInterpolation( section )
      };
      xs.insert( std::make_pair( MT2ReactionID( section.MT() ), 
                                 std::move( reaction ) ) );
    }
  } else {
    Log::info( "MF=13 doesn't exist. No photon production data to extract." );
  }
  
  return xs;
}
