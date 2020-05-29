void header( Tape_t&, std::string&& text ){
  auto tpid = ENDFtk::TPID( std::move( text ), 1 );
  tpid.print( this->ipendf, 0, 0, 0 );
}

void header( std::string&& text ){
  Log::info( "eval type: {}", typeid( this->evaluated ).name() );
  std::visit(
    [&]( auto&& eval ){ 
      return this->header( eval, std::move( text ) ); },
    this->evaluated
  );
}
