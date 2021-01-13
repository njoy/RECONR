void footer( const Tape_t&, std::string&& ){
  ENDFtk::TEND().print( this->ipendf );
}

void footer( std::string&& text="" ){
  std::visit(
    [&]( auto&& eval ){ return this->footer( eval, std::move( text ) ); },
    this->evaluated
  );
}
