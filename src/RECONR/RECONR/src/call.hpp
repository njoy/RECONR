void operator()( const nlohmann::json& njoyArgs, 
                 std::ostream& output,
                 const nlohmann::json& ){

  output << "Input arguments:\n" << njoyArgs.dump( 2 ) << std::endl;

  std::string filename = "tape" + std::to_string( njoyArgs[ "npend" ].get< int >() );
  std::ofstream pendf( filename );
  std::ostreambuf_iterator< char > ipendf( pendf );

  auto tpid = ENDFtk::TPID( njoyArgs[ "tlabel" ].get< std::string >(), 1 );
  tpid.print( ipendf, 0, 0, 0 );

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

    long size = energies.size();
    std::vector< ENDFtk::section::Type< 3 > > sections;
    for( auto& [ MT, rx ] : reactions ){
      std::vector< long > boundaries{ size };
      std::vector< long > interpolants{ 2 };

      sections.emplace_back( MT, rx.ZA(), rx.AWR(), rx.QM(), rx.QI(), rx.LR(),
                             std::move( boundaries ), std::move( interpolants ),
                             utility::copy( energies ), 
                             utility::copy( rx.crossSections() ) );
    }
    ENDFtk::file::Type< 3 > mf3{ std::move( sections ) };
    mf3.print( ipendf, MAT );

    ENDFtk::MEND().print( ipendf );
  }

  ENDFtk::TEND().print( ipendf );
  pendf.close();
}
