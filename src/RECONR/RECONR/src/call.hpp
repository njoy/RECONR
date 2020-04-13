void operator()( const nlohmann::json& njoyArgs, const nlohmann::json& ){
  Map_t materialMap{};

  auto evaluatedData = getEvaluated( njoyArgs[ "nendf" ] );
  for( auto& sequence : njoyArgs[ "sequence" ] ){
    auto data = this->findR2D2( sequence, evaluatedData );
    this->linearize( data, sequence[ "err" ], 1E-7 );
    materialMap.emplace( sequence[ "mat" ], std::move( data ) );
  }
}
