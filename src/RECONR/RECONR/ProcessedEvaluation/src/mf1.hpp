void mf1( const int& MAT, const nlohmann::json& sequence ){
  using namespace njoy::ENDFtk::literals;

  auto eval = std::get< 0 >( this->evaluated );
  const auto oldMF1 = eval.materialNumber( MAT ).front()
                       .fileNumber( 1 ).parse< 1 >();
  const auto mt = oldMF1.section( 451_c );

  auto cards = sequence[ "cards" ].get< std::vector< std::string > >();
  auto NWD = ranges::distance( cards );
  std::vector< ENDFtk::TEXT > description;
  for( auto& card : cards ){ 
    description.emplace_back( std::move( card ) );
  }

  std::vector< ENDFtk::DIR > directory;
  long NXC = directory.size();

  std::array< ENDFtk::CONT, 3 > parameters{
    ENDFtk::CONT{ mt.ELIS(), mt.STA(), mt.LIS(), mt.LISO(), 0, mt.NFOR() }, 
    ENDFtk::CONT{ mt.AWI(), mt.EMAX(), mt.LREL(), 0, mt.NSUB(), mt.NVER() }, 
    ENDFtk::CONT{ mt.TEMP(), 1E-3, mt.LDRV(), 0, NWD, NXC       }
  };
  ENDFtk::section::Type< 1, 451 > mt451( 
    // --------------------------------------
    mt.ZA(), mt.AWR(), mt.LRP(), mt.LFI(), mt.NLIB(), mt.NMOD(),
    std::move( parameters ),
    std::move( description ),
    std::move( directory )
  );

  mt451.print( this->ipendf, MAT, 1 );
  ENDFtk::FEND( MAT ).print( this->ipendf );
}
