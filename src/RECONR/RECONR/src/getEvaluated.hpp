static
auto getEvaluated( int nendf ){

  std::string filename = "tape" + std::to_string( nendf );
  buffer_t ENDF;
  try {
    ENDF = njoy::utility::slurpFileToMemory( filename );
  } catch( ... ){
    Log::info( "Error ocurred when trying to open evaluated data file: {}.",
               filename);
    throw;
  }
  // return njoy::ENDFtk::syntaxTree::Tape< buffer_t >( std::move( ENDF ) );
  auto tape = njoy::ENDFtk::syntaxTree::makeTape( std::move( ENDF ) );
  return tape;
}
