static
Format_t getEvaluated( std::ostream& output, int nendf ){

  std::string filename = "tape" + std::to_string( nendf );
  output << fmt::format( "\nReading evaluated data from file: {}", filename )
         << std::endl;
  try {
    auto evaluated = njoy::utility::slurpFileToMemory( filename );
    return Format_t{ std::move( evaluated ) };
  } catch( ... ){
    Log::info( "Error ocurred when trying to open evaluated data file: {}.",
               filename );
    throw;
  }
}
