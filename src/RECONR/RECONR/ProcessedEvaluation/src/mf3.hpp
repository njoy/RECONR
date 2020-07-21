template< typename Reaction_t >
ENDFtk::file::Type< 3 >
mf3( const int& MAT,
     const Reaction_t& reactions ){

  std::vector< ENDFtk::section::Type< 3 > > sections;
  for( auto& [ ID, rx ] : reactions ){
    // if( ID == "27" ){
    //   Log::info( "Intentionally not including MT=27 in ENDF file" );
    //   continue;
    // }
    // if( ID == "101" ){
    //   Log::info( "Intentionally not including MT=101 in ENDF file" );
    //   continue;
    // }

    auto [ energies, crossSections ] = rx.template crossSections< XSPair >();

    std::vector< long > boundaries{ ranges::distance( energies ) };
    std::vector< long > interpolants{ 2 };

    sections.emplace_back( elementary::toEndfReactionNumber( ID ),
                           rx.ZA(), rx.AWR(), rx.QM(), rx.QI(), rx.LR(),
                           std::move( boundaries ), std::move( interpolants ),
                           std::move( energies ),
                           std::move( crossSections )
                          );
  }

  return ENDFtk::file::Type< 3 >{ std::move( sections ) };
}
