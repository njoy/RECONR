static
Format_t getEvaluated( const Logger& logger, std::string filename ){

  logger.first << fmt::format( 
      "\nReading evaluated data from file: {}", filename )
         << std::endl;
  try {
    return ENDFtk::tree::fromFile( filename );
  } catch( ... ){
    Log::info( "Error ocurred when trying to open evaluated data file: {}.",
               filename );
    throw;
  }
}
