void operator()( const nlohmann::json& njoyArgs, 
                 std::ostream& output,
                 const nlohmann::json& ){

  output << "Input arguments:\n" << njoyArgs.dump( 2 ) << std::endl;
  auto filename = "tape" + std::to_string( njoyArgs[ "npend" ].get< int >() );

  auto evaluatedData = getEvaluated( output, njoyArgs[ "nendf" ] );

  ProcessedEvaluation pendf( evaluatedData, filename );
  pendf.header( njoyArgs[ "tlabel" ].get< std::string >() );

  for( auto& sequence : njoyArgs[ "sequence" ] ){
    int MAT = sequence[ "mat" ];
    output << "\n------------------------------------------------------\n";
    output <<  fmt::format("Processing ENDF Material: {}\n", MAT );

    auto err = sequence[ "err" ];

    auto data = this->findR2D2( output, sequence, evaluatedData );
    this->linearizeXS( output, data, err, this->absoluteTolerance );
    // Get unionized energy grid
    std::vector< double > enode = sequence.at( "enode" );
    auto grid = this->unionizeEnergyGrid( output, data, enode );
    // Reconstruct resonances
    this->reconstructResonances( 
      output, grid, data, err, this->absoluteTolerance );
    // Recalculate linearized cross sections
    auto energies = this->unionizeEnergyGrid( output, data );
    // Sum reactions
    auto reactions = this->summateReactions( output, data, energies );

    pendf.material( MAT, data, reactions, energies, sequence );

  }

  pendf.footer();
}

