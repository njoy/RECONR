void processMaterial( const nlohmann::json& sequence ){
  std::visit( 
    [&]( auto&& arg ){ return this->processMaterial( sequence, arg ); },
    this->evaluatedData
  );
}

void processMaterial( const nlohmann::json& sequence, const Tape_t& tape ){
  Log::info( "In processMaterial with sequnce:{}", sequence.dump(2) );

  auto material = tape.materialNumber( sequence[ "mat" ] );
  processMaterial( sequence, material );
}

void processMaterial( const nlohmann::json& , 
                      const Material_t&  ){
}
