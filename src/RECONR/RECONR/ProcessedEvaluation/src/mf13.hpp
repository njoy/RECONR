std::optional< ENDFtk::file::Type< 13 > >
mf13( const Logger& logger, const int& MAT, const R2D2& data ){

  const auto& productions = data.photonProductions();
  if( productions.empty() ){ return std::nullopt; }

  logger.first << "Preparing MF=13 data." << std::endl;
  std::vector< ENDFtk::section::Type< 13 > > sections;

  for( auto& [ ID, production ] : data.photonProductions() ){
    auto mt = elementary::toEndfReactionNumber( ID );
    logger.first << fmt::format( "\t{:3} {:s}\n", mt, ID.symbol() );

    auto prods = production.template productions< PPair >();
    std::vector< long > boundaries{ static_cast< long >( prods.size() ) };
    std::vector< long > interpolants{ 2 };

    auto energies = prods
      | ranges::view::transform( []( auto&& p ){ return p.first; } )
      | ranges::to_vector;
    auto photons = prods
      | ranges::view::transform( []( auto&& p ){ return p.second; } )
      | ranges::to_vector;
    ENDFtk::section::Type< 13 >::PartialCrossSection partial{
      0.0, 0.0, 0, 1,
      std::move( boundaries ),
      std::move( interpolants ),
      std::move( energies ),
      std::move( photons )
    };

    sections.emplace_back( elementary::toEndfReactionNumber( ID ),
                           production.ZA(), production.AWR(), 
                           std::move( partial ) );
    
  }

  return ENDFtk::file::Type< 13 >{ std::move( sections ) };

}
