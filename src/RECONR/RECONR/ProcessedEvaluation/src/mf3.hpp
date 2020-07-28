ENDFtk::file::Type< 3 >
mf3( const int& MAT, const R2D2& data ){

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

  for( auto& [ ID, rx ] : data.reactions() ){ 
    auto mt = elementary::toEndfReactionNumber( ID );
    Log::info( "ID: {:25s}, MT: {:4}", ID.symbol(), mt );
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

  for( auto& [ ID, rx ] : data.summations() ){ 
    auto mt = elementary::toEndfReactionNumber( ID );
    if( p2t.count( mt ) ){ mt = p2t.at( mt ); }
    Log::info( "ID: {:25s}, MT: {:4}", ID.symbol(), mt );
    addSection( mt, rx );
  }

  return ENDFtk::file::Type< 3 >{ std::move( sections ) };
}
