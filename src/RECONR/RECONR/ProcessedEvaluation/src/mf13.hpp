template< typename Production_T >
std::optional< ENDFtk::file::Type< 13 > >
mf13( const int& MAT, const Production_T& productions ){

  std::vector< ENDFtk::section::Type< 13 > > sections;
  for( auto& [ MT, production ] : productions ){

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

    sections.emplace_back( reactionID2MT( MT ), 
                           production.ZA(), production.AWR(), 
                           std::move( partial ) );
    
  }

  if( sections.empty() ){
    return std::nullopt;
  } else{
    return ENDFtk::file::Type< 13 >{ std::move( sections ) };
  }

}
