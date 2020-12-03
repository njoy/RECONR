R2D2 findR2D2( const Logger& logger,
               const nlohmann::json& sequence, 
               const Tape_t& tape ){
  auto& output = logger.first;
  output << "\nExtracting resonance reconstruction data from an ENDF file."
         << std::endl;
  try {
    auto material = tape.material( sequence[ "mat" ] ).front();
    return R2D2::Factory()( logger, material );

  } catch( ... ){
    Log::info( "Error while extracting material from ENDF Tape." );
    throw;
  }
}

R2D2 findR2D2( const Logger& logger,
               const nlohmann::json& sequence, 
               Format_t evaluatedData ){
  return std::visit( 
    [&]( auto&& arg ){ return this->findR2D2( logger, sequence, arg ); },
    evaluatedData
  );
}
