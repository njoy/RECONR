void operator()( const nlohmann::json& njoyArgs, const nlohmann::json& ){
  auto evaluatedData = getEvaluated( njoyArgs[ "nendf" ] );
  for( auto& sequence : njoyArgs[ "sequence" ] ){
    // auto data = this->collectData( sequence );
    // this-?processData( data );
  }
}
