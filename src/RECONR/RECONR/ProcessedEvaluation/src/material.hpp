template< typename Reaction_t, typename Production_t, typename Energies_t >
void material( const Tape_t&, 
               const int& MAT,
               const R2D2& data,
               const Reaction_t& reactions, 
               const Production_t& productions,
               const Energies_t& energies,
               const nlohmann::json& sequence ){

  auto MF1  = this->mf1( MAT, sequence );
  auto MF2  = this->mf2( MAT, data );
  auto MF3  = this->mf3( MAT, reactions, energies );
  auto MF13 = this->mf13( MAT, productions, energies );

  MF1.print( this->ipendf, MAT, 1 );
  MF2.print( this->ipendf, MAT );
  MF3.print( this->ipendf, MAT );
  MF13.print( this->ipendf, MAT );
  ENDFtk::MEND().print( this->ipendf );
}

template< typename Reaction_t, typename Production_t, typename Energies_t >
void material( int& MAT, 
               const R2D2& data,
               const Reaction_t& reactions, 
               const Production_t& productions,
               const Energies_t& energies,
               const nlohmann::json& sequence){
  std::visit(
    [&]( auto&& eval ){ 
      return this->material( eval,
                             MAT,
                             data,
                             reactions,
                             productions,
                             energies,
                             sequence ); 
    },
    this->evaluated
  );
}

