auto mf1( const int& MAT, const nlohmann::json& sequence,
          const ENDFtk::file::Type< 2 >&  mf2,
          const ENDFtk::file::Type< 3 >&  mf3,
          const ENDFtk::file::Type< 13 >& mf13
         ){
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
