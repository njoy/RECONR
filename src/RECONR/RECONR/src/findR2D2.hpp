R2D2 findR2D2( std::ostream& output,
               const nlohmann::json& sequence, 
               const Tape_t& tape ){
  output << "\nExtracting resonance reconstruction data from an ENDF file."
         << std::endl;
  try {
    auto material = tape.materialNumber( sequence[ "mat" ] ).front();
    return R2D2::Factory()( material );

  } catch( ... ){
    Log::info( "Error while extracting material from ENDF Tape." );
    throw;
  }
}

R2D2 findR2D2( std::ostream& output,  
               const nlohmann::json& sequence, 
               Format_t evaluatedData ){
  return std::visit( 
    [&]( auto&& arg ){ return this->findR2D2( output, sequence, arg ); },
    evaluatedData
  );
}
