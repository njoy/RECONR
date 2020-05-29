static
Format_t getEvaluated( int nendf ){

  std::string filename = "tape" + std::to_string( nendf );
  std::string evaluated;
  try {
    evaluated = njoy::utility::slurpFileToMemory( filename );
  } catch( ... ){
    Log::info( "Error ocurred when trying to open evaluated data file: {}.",
               filename);
    throw;
  }
  Tape_t tape = njoy::ENDFtk::syntaxTree::makeTape( std::move( evaluated ) );
  return tape;
}
