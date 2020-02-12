void linearize(){
  Log::info( "Linearizing R2D2" );

  for( auto reaction : this->crossSections_ ){
    auto MT = std::get< 0 >( reaction );
    auto XS = std::get< 1 >( reaction );

    Log::info( "linearizing reaction: {}", MT );
  }
}
