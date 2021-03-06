ENDFtk::file::Type< 3 >
mf3( const Logger& logger, const R2D2& data ){

  logger.first << "Preparing MF=3 data." << std::endl;
  std::vector< ENDFtk::section::Type< 3 > > sections;

  auto addSection = [&]( auto&& mt, auto&& rx ){
    auto [ energies, crossSections ] = rx.template crossSections< XSPair >();

    std::vector< long > boundaries{ ranges::distance( energies ) };
    std::vector< long > interpolants{ 2 };

    sections.emplace_back( mt, 
                           rx.ZA(), rx.AWR(), rx.QM(), rx.QI(), rx.LR(),
                           std::move( boundaries ), std::move( interpolants ),
                           std::move( energies ),
                           std::move( crossSections )
                          );
  };

  auto MTs = data.MF3MTs();
  auto inENDF = [&]( auto&& mt ){ 
    return ranges::find( MTs, mt ) != MTs.end(); };

  logger.first << "MF=3 reactions: " << std::endl;
  for( auto& [ ID, rx ] : data.reactions() ){ 
    auto mt = elementary::toEndfReactionNumber( ID );

    // We'll take care of this in the summations
    if( mt == 1 ){ continue; }
    if( not inENDF( mt ) ){ continue; }

    logger.first << fmt::format( "\t{:3} {:s}\n", mt, ID.symbol() );

    addSection( mt, rx );
  }

  // Partial to total convsersion *hack*
  static const std::map< int, int > p2t{
    { 600, 103 },
    { 650, 104 },
    { 700, 105 },
    { 750, 106 },
    { 800, 107 },
    { 875,  16 },
  };

  logger.first << "MF=3 redundant (summation) reactions: " << std::endl;
  for( auto& [ ID, rx ] : data.summations() ){ 
    auto mt = elementary::toEndfReactionNumber( ID );
    logger.first << fmt::format( "\t{:3} {:s}\n", mt, ID.symbol() );
    if( p2t.count( mt ) ){ mt = p2t.at( mt ); }
    addSection( mt, rx );
  }

  return ENDFtk::file::Type< 3 >{ std::move( sections ) };
}
