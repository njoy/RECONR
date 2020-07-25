void operator()( const nlohmann::json& njoyArgs, 
                 std::ostream& output,
                 std::ostream& error,
                 const nlohmann::json& ){

  output << "Input arguments:\n" << njoyArgs.dump( 2 ) << std::endl;
  std::string inputFilename = "tape" + 
    std::to_string( njoyArgs[ "nendf" ].get< int >() );
  auto outputFilename = "tape" + 
    std::to_string( njoyArgs[ "npend" ].get< int >() );

  auto evaluatedData = getEvaluated( output, inputFilename );

  ProcessedEvaluation pendf( evaluatedData, outputFilename );
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
                                          data.reactions(), 
                                          data.photonProductions(),
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
    this->reconstructCrossSections( output, error, data, energies );
    this->combineReconstructed( output, error, data, energies );
    this->summateReactions( output, error, data, energies );
    this->summateProductions( output, error, data, energies );

    // Remove leading zeros
    this->truncateReactions( output, data );

    // pendf.material( MAT, data, sequence );

  }

  pendf.footer();
}

/*
 * This is the modern interface
 */
void operator()( std::ostream& output, std::ostream& error, 
                 const nlohmann::json& arguments ){

  output << "Modern RECONR interface.\n"
         << "Input arguments: \n" << arguments.dump( 2 ) << std::endl;

  output << "This doesn't do anything at the moment." << std::endl;

  throw std::exception();
}
