void material( std::ostream& output, 
               std::ostream& error,
               const Tape_t&, 
               const int& MAT,
               const R2D2& data,
               const nlohmann::json& sequence )  {

  output << "Preparing data to be written to PENDF file." << std::endl;

  auto MF2  = this->mf2( output, error, MAT, data );
  auto MF3  = this->mf3( output, error, MAT, data );
  auto MF13 = this->mf13( output, error, MAT, data );
  auto MF1  = this->mf1( output, error, MAT, sequence, MF2, MF3, MF13 );

  output << "\nWriting PENDF data to file." << std::endl;
  MF1.print( this->ipendf, MAT, 1 );
  ENDFtk::FEND( MAT ).print( this->ipendf );

  MF2.print( this->ipendf, MAT );
  MF3.print( this->ipendf, MAT );
  if( MF13 ){
    MF13->print( this->ipendf, MAT );
  }
  ENDFtk::MEND().print( this->ipendf );
}

void material( std::ostream& output, 
               std::ostream& error,
               int& MAT, 
               const R2D2& data,
               const nlohmann::json& sequence){
  std::visit(
    [&]( auto&& eval ){ 
      return this->material( output, error, eval, MAT, data, sequence ); },
    this->evaluated
  );
}

