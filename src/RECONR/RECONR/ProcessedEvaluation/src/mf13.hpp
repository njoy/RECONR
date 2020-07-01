template< typename Production_T >
ENDFtk::file::Type< 13 >
mf13( const int& MAT,
     const Production_T& productions ){

  std::vector< ENDFtk::section::Type< 13 > > sections;
  for( auto& [ MT, production ] : productions ){

    // std::vector< long > boundaries{ ranges::distance( production ) };
    // std::vector< long > interpolants{ 2 };

    // ENDFtk::section::Type< 13 >::PartialCrossSection partial{
    //   0.0, 0.0, 0, 1,
    //   std::move( boundaries ),
    //   std::move( interpolants ),
    //   utility::copy( energies ),
    //   utility::copy( production.productions() )
    // };
    
  }

  return ENDFtk::file::Type< 13 >{ std::move( sections ) };

}
