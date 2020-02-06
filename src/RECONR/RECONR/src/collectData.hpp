void collectData( const nlohmann::json& sequence, Format_t evaluatedData ){
  std::visit( 
    [&]( auto&& arg ){ return this->collectData( sequence, arg ); },
    evaluatedData
  );
}

void collectData( const nlohmann::json& sequence, const Tape_t& tape ){

  try {
    tape.materialNumber( sequence[ "mat" ] );
  } catch( ... ){
    Log::info( "Error while extracting material from ENDF Tape." );
    throw;
  }
  // processMaterial( sequence, material );
}
