R2D2 findR2D2( const nlohmann::json& sequence, Tape_t& tape ){

  try {
    return R2D2{ tape.materialNumber( sequence[ "mat" ] ).front() };
  } catch( ... ){
    Log::info( "Error while extracting material from ENDF Tape." );
    throw;
  }
}

R2D2 findR2D2( const nlohmann::json& sequence, Format_t evaluatedData ){
  return std::visit( 
    [&]( auto&& arg ){ return this->findR2D2( sequence, arg ); },
    evaluatedData
  );
}
