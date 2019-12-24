RECONR( nlohmann::json&& njoyArgs, const nlohmann::json& args )
  try:
    njoyArgs( std::move( njoyArgs ) ),
    args( args ),
    evaluated( getEvaluated( ) ){
  } catch( ... ){
    Log::info( "Trouble creating a RECONR object." );
    throw;
  }
