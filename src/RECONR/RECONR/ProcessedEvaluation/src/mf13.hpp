template< typename Production_T, typename Energies_t >
ENDFtk::file::Type< 13 >
mf13( const int& MAT,
     const Production_T& productions, 
     const Energies_t& energies ){

  long size = energies.size();

  std::vector< ENDFtk::section::Type< 13 > > sections;
  for( auto& [ MT, production ] : productions ){

    std::vector< long > boundaries{ size };
    std::vector< long > interpolants{ 2 };

    // section::Type< 13 >::PartialCrossSection partial{

    //   std::move( boundaries ),
    //   std::move( interpolants ),
    //   std::move( production.productions() ),
    //   std::move( utility::copy( energies ) )
    // };
    
  }

  return ENDFtk::file::Type< 13 >{ std::move( sections ) };

}
