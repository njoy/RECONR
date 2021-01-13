void header( Tape_t&, std::string&& text ){
  auto tpid = ENDFtk::TPID( fmt::format( "{:<66s}", text ), 1 );
  tpid.print( this->ipendf, 1, 0, 0 );
}

void header( std::string&& text ){
  std::visit(
    [&]( auto&& eval ){ 
      return this->header( eval, std::move( text ) ); },
    this->evaluated
  );
}
