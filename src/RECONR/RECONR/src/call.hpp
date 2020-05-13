void operator()( const nlohmann::json& njoyArgs, const nlohmann::json& ){
  Map_t materialMap{};

  auto evaluatedData = getEvaluated( njoyArgs[ "nendf" ] );
  for( auto& sequence : njoyArgs[ "sequence" ] ){

    auto err = sequence[ "err" ];

    auto data = this->findR2D2( sequence, evaluatedData );
    this->linearizeXS( data, err, this->absoluteTolerance );
    // Get unionized energy grid
    auto grid = this->unionizeEnergyGrid( data, sequence[ "enode" ] );
    // Reconstruct resonances
    this->reconstructResonances( grid, data, err, this->absoluteTolerance );
    // Recalculate linearized cross sections
    auto energies = this->unionizeEnergyGrid( data );
    // Sum reactions
    auto reactions = this->summateReactions( data, energies );
    materialMap.emplace( sequence[ "mat" ], std::move( data ) );
  }
}
