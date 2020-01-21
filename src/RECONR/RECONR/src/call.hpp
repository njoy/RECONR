void operator()(){
  for( auto& sequence : this->njoyArgs[ "sequence" ] ){
    this->processMaterial( sequence );
  }
}
