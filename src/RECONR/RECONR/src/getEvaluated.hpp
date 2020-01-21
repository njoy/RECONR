Format_t getEvaluated( ){

  std::string filename = "tape" + std::to_string( 
      this->njoyArgs.at( "nendf" ).get< int >() );
  buffer_t ENDF;
  try {
    ENDF = njoy::utility::slurpFileToMemory( filename );
  } catch( ... ){
    Log::info( "Error ocurred when trying to open evaluated data file: {}.",
               filename);
  }

  return njoy::ENDFtk::syntaxTree::makeTape( std::move( ENDF ) );

}
