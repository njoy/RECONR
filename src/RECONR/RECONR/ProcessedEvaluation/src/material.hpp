template< typename Reaction_t, typename Energies_t >
void material( const Tape_t&, 
               const int& MAT,
               const R2D2& data,
               const Reaction_t& reactions, 
               const Energies_t& energies,
               const nlohmann::json& sequence ){

  this->mf1( MAT, sequence );
  this->mf2( MAT, data );
  this->mf3( MAT, reactions, energies );

  ENDFtk::MEND().print( this->ipendf );
}

template< typename Reaction_t, typename Energies_t >
void material( int& MAT, 
               const R2D2& data,
               const Reaction_t& reactions, 
               const Energies_t& energies,
               const nlohmann::json& sequence){
  std::visit(
    [&]( auto&& eval ){ 
      return this->material( eval, MAT, data, reactions, energies, sequence ); 
    },
    this->evaluated
  );
}

