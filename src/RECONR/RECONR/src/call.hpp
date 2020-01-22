void operator()(){
  for( auto& sequence : this->njoyArgs[ "sequence" ] ){
    auto data = this->collectData( sequence );
    // this-?processData( data );
  }
}
