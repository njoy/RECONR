auto collectData( const nlohmann::json& sequence ){
  std::visit( 
    [&]( auto&& arg ){ return this->processMaterial( sequence, arg ); },
    this->evaluatedData
  );
}

auto collectData( const nlohmann::json& sequence, const Tape_t& tape ){

  try {
    auto material = tape.materialNumber( sequence[ "mat" ] );
  } catch( ... ){
    Log::info( "Error while extracting material from ENDF Tape." );
    throw;
  }
  // processMaterial( sequence, material );
}
