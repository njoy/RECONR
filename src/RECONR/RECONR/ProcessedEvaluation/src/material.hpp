template< typename Reaction_t, typename Energies_t >
void material( const Tape_t&, 
               const int& MAT,
               const Reaction_t& reactions, 
               const Energies_t& energies ){

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
  mf3.print( this->ipendf, MAT );
  ENDFtk::MEND().print( this->ipendf );
}

template< typename Reaction_t, typename Energies_t >
void material( int& MAT, 
               const Reaction_t& reactions, 
               const Energies_t& energies ){
  std::visit(
    [&]( auto&& eval ){ 
      return this->material( eval, MAT, reactions, energies ); 
    },
    this->evaluated
  );
}

