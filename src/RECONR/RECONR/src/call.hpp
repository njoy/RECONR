void operator()( const nlohmann::json& njoyArgs, 
                 std::ostream& output,
                 std::ostream& error,
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
    auto grid = this->unionizeEnergyGrid( output, 
                                          data.linearReactions(), 
                                          data.linearPhotonProductions(),
                                          data.resonanceReferenceGrid(),
                                          enode );
    auto gridSize = ranges::distance( grid );

    output << fmt::format(
      std::string{ "number of user nodes               = {:10d}\n" } +
      std::string{ "number of user and resonance nodes = {:10d}\n" }, 
      ranges::distance( enode ), gridSize )
           << std::endl;

    // Reconstruct resonances
    this->reconstructResonances( 
      output, grid, data, err, this->absoluteTolerance );
    // Recalculate linearized cross sections
    auto energies = this->unionizeEnergyGrid( output, 
                                              grid,
                                              data.reconstructedResonances() );
    auto eSize = ranges::distance( energies );

    output <<  fmt::format(
      "number of points in final unionized grid    = {:10d}\n", eSize )
           << std::endl;

    // Sum reactions
    // auto reactions = this->summateReactions( output, data, energies );
    auto summedReactions = this->summateReactions( 
      output, data.linearReactions(), 
      data.reconstructedResonances(), energies );
    auto summedProductions = this->summateReactions( 
      output, data.linearPhotonProductions(), energies );

    // Remove leading/trailing zeros
    auto truncReactions = this->truncateReactions( output, energies, 
                                                  summedReactions );
    Log::info( "calling truncate for photon productions" );
    auto truncProductions = this->truncateReactions( output, energies, 
                                                    summedProductions );
    pendf.material( MAT, data, 
                    truncReactions, 
                    truncProductions, 
                    sequence );

  }

  pendf.footer();
}

