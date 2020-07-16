auto mf1( const int& MAT, const nlohmann::json& sequence,
          const ENDFtk::file::Type< 2 >&  mf2,
          const ENDFtk::file::Type< 3 >&  mf3,
          const std::optional< ENDFtk::file::Type< 13 > >& mf13
         ){
  using namespace njoy::ENDFtk::literals;

  auto eval = std::get< 0 >( this->evaluated );
  const auto mt = eval.materialNumber( MAT ).front()
                       .fileNumber( 1 ).sectionNumber( 451 ).parse< 1, 451 >();

  auto cards = sequence[ "cards" ].get< std::vector< std::string > >();
  long NWD = cards.size();
  std::vector< ENDFtk::TEXT > description;
  for( auto& card : cards ){ 
    // Make right-padded
    description.emplace_back( fmt::format( "{:<66s}", card ) );
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
  //                                   MF,  MT, NC
  directory.emplace( directory.begin(), 1, 451, directory.size() + 1 + 4, 0 );
  long NXC = directory.size();

  std::array< ENDFtk::CONT, 3 > parameters{
    ENDFtk::CONT{ mt.ELIS(), mt.STA(), mt.LIS(), mt.LISO(), 0, mt.NFOR() }, 
    ENDFtk::CONT{ mt.AWI(), mt.EMAX(), mt.LREL(), 0, mt.NSUB(), mt.NVER() }, 
    ENDFtk::CONT{ mt.TEMP(), sequence[ "err" ], mt.LDRV(), 0, NWD, NXC       }
  };

  ENDFtk::section::Type< 1, 451 > mt451( 
    mt.ZA(), mt.AWR(), mt.LRP(), mt.LFI(), mt.NLIB(), mt.NMOD(),
    std::move( parameters ),
    std::move( description ),
    std::move( directory )
  );

  return mt451;
}
