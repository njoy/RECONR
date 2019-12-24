Format_t getEvaluated( ){

  std::string filename = "tape" + std::to_string( 
      this->njoyArgs.at( "nendf" ).get< int >() );
  Log::info( "filename: {}", filename );
  auto ENDF = njoy::utility::slurpFileToMemory( filename );
  Log::info( "slurped filename" );

  return njoy::ENDFtk::syntaxTree::makeTape( std::move( ENDF ) );
}
