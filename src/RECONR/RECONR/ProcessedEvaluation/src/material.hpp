void material( const Tape_t&, 
               const int& MAT,
               const R2D2& data,
               const nlohmann::json& sequence )  {

  auto MF2  = this->mf2( MAT, data );
  auto MF3  = this->mf3( MAT, data );
  auto MF13 = this->mf13( MAT, data );
  auto MF1  = this->mf1( MAT, sequence, MF2, MF3, MF13 );

  MF1.print( this->ipendf, MAT, 1 );
  ENDFtk::FEND( MAT ).print( this->ipendf );

  MF2.print( this->ipendf, MAT );
  MF3.print( this->ipendf, MAT );
  if( MF13 ){
    Log::info( "Printing MF=13." );
    MF13->print( this->ipendf, MAT );
  }
  ENDFtk::MEND().print( this->ipendf );
}

void material( int& MAT, 
               const R2D2& data,
               const nlohmann::json& sequence){
  std::visit(
    [&]( auto&& eval ){ return this->material( eval, MAT, data, sequence ); },
    this->evaluated
  );
}

