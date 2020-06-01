static
Format_t getEvaluated( int nendf ){

  std::string filename = "tape" + std::to_string( nendf );
  try {
    auto evaluated = njoy::utility::slurpFileToMemory( filename );
    return Format_t{ std::move( evaluated ) };
  } catch( ... ){
    Log::info( "Error ocurred when trying to open evaluated data file: {}.",
               filename );
    throw;
  }
}
