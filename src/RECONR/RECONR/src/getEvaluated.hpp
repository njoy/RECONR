static
Format_t getEvaluated( std::ostream& output, std::string filename ){

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
