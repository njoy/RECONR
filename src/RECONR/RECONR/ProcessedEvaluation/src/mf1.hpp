auto mf1( const Logger& logger,
          const int& MAT, const nlohmann::json& sequence,
          const ENDFtk::file::Type< 2 >&  mf2,
          const ENDFtk::file::Type< 3 >&  mf3,
          const std::optional< ENDFtk::file::Type< 13 > >& mf13
         ){
  using namespace njoy::ENDFtk::literals;

  logger.first << "Preparing MF=1 data." << std::endl;

  auto eval = std::get< 0 >( this->evaluated );
  const auto mt = eval.material( MAT ).front()
                       .file( 1 ).section( 451 ).parse< 1, 451 >();

  auto cards = sequence[ "cards" ].get< std::vector< std::string > >();
  std::string description;
  for( auto& card : cards ){ 
    // Make right-padded
    description += fmt::format( "{:<66s}\n", card );
  }

  std::vector< ENDFtk::DIR > directory;

  directory.emplace_back( 2, 151, mf2.section( 151_c ).NC(), 0 );
  if( mf2.hasSection( 152 ) ){
    directory.emplace_back( 2, 152, mf2.section( 152_c ).NC(), 0 );
  }
  for( const auto& section : mf3.sections() ){
    directory.emplace_back( 3, section.MT(), section.NC(), 0 );
  }
  if( mf13 ){
    for( const auto& section : mf13->sections() ){
      directory.emplace_back( 13, section.MT(), section.NC(), 0 );
    }
  }
  //                                   MF,  MT, | <------- NC -------> |
  directory.emplace( directory.begin(), 1, 451, directory.size() + 1 + 4, 0 );

  double err = sequence[ "err" ];
  ENDFtk::section::Type< 1, 451 > mt451( 
    mt.ZA(),    mt.AWR(),   mt.LRP(),   mt.LFI(),   mt.NLIB(),  mt.NMOD(),  
    mt.ELIS(),  mt.STA(),   mt.LIS(),   mt.LISO(),  mt.NFOR(),  
    mt.AWI(),   mt.EMAX(),  mt.LREL(),  mt.NSUB(),  mt.NVER(),  
    mt.TEMP(),  mt.LDRV(),  
    std::move( description ),
    std::move( directory ),
    err // rtol
  );

  return mt451;
}
