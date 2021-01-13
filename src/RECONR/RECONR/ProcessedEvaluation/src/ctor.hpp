ProcessedEvaluation( Format_t& evaluated, std::string filename )
try:
  evaluated( evaluated ),
  pendf( filename ),
  ipendf( this->pendf )
{ } catch( ... ){
  Log::info( "Trouble creating ProcessedEvaluation (PENDF) object." );
  throw;
}
