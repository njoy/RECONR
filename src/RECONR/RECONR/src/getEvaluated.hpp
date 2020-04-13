static Format_t getEvaluated( int nendf ){

  std::string filename = "tape" + std::to_string( nendf );
  buffer_t ENDF;
  try {
    ENDF = njoy::utility::slurpFileToMemory( filename );
  } catch( ... ){
    Log::info( "Error ocurred when trying to open evaluated data file: {}.",
               filename);
  }
  // return njoy::ENDFtk::syntaxTree::Tape< buffer_t >( std::move( ENDF ) );
  return Format_t{ njoy::ENDFtk::syntaxTree::makeTape( std::move( ENDF ) ) };
}
