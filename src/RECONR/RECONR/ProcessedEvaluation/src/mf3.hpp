template< typename Reaction_t, typename Energies_t >
void mf3( const int& MAT,
          const Reaction_t& reactions, 
          const Energies_t& energies ){

  long size = energies.size();
  std::vector< ENDFtk::section::Type< 3 > > sections;
  for( auto& [ MT, rx ] : reactions ){
    if( MT == 27 ){
      Log::info( "Intentionally not including MT=27 in ENDF file" );
      continue;
    }
    if( MT == 101 ){
      Log::info( "Intentionally not including MT=101 in ENDF file" );
      continue;
    }

    std::vector< long > boundaries{ size };
    std::vector< long > interpolants{ 2 };

    sections.emplace_back( MT, rx.ZA(), rx.AWR(), rx.QM(), rx.QI(), rx.LR(),
                            std::move( boundaries ), std::move( interpolants ),
                            utility::copy( energies ), 
                            utility::copy( rx.crossSections() ) );
  }
  ENDFtk::file::Type< 3 > mf3{ std::move( sections ) };
  mf3.print( this->ipendf, MAT );
}
