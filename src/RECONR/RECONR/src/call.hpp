void operator()( const nlohmann::json& njoyArgs, const nlohmann::json& ){
  Map_t materialMap{};

  auto evaluatedData = getEvaluated( njoyArgs[ "nendf" ] );
  for( auto& sequence : njoyArgs[ "sequence" ] ){
    auto data = this->findR2D2( sequence, evaluatedData );
    this->linearizeXS( data, sequence[ "err" ], 1E-7 );
    // Get unionized energy grid
    // Reconstruct resonances
    // Recalculate linearized cross sections
    // Sum reactions
        // Add resonances to appropriate cross sections
    materialMap.emplace( sequence[ "mat" ], std::move( data ) );
  }
}
