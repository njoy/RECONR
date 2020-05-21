void operator()( const nlohmann::json& njoyArgs, const nlohmann::json& ){
  Map_t materialMap{};

  auto evaluatedData = getEvaluated( njoyArgs[ "nendf" ] );
  for( auto& sequence : njoyArgs[ "sequence" ] ){
    auto MAT = sequence[ "mat" ];
    Log::info( "Processing ENDF Material: {}", MAT );

    auto err = sequence[ "err" ];

    auto data = this->findR2D2( sequence, evaluatedData );
    this->linearizeXS( data, err, this->absoluteTolerance );
    // Get unionized energy grid
    std::vector< double > enode = sequence.at( "enode" );
    auto grid = this->unionizeEnergyGrid( data, enode );
    // Reconstruct resonances
    this->reconstructResonances( grid, data, err, this->absoluteTolerance );
    // Recalculate linearized cross sections
    auto energies = this->unionizeEnergyGrid( data );
    // Sum reactions
    auto reactions = this->summateReactions( data, energies );
    materialMap.emplace( MAT, std::move( reactions ) );
  }
}
