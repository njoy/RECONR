ENDFtk::file::Type< 2 >
mf2( const Logger& logger, const R2D2& data ){

  logger.first << "Preparing MF=2 data." << std::endl;
  logger.first << "\tMT=151" << std::endl;
  // MT151
  // We're going to assume that everything we see only has one resolved range
  auto params = std::get< 0 >( data.resonanceParameters() );
  auto iso = params.isotopes().front();
  auto range = iso.resonanceRanges().front();

  auto [SPI, AP] = std::visit(
    [&]( auto& form ){ return std::make_pair( form.SPI(), form.AP() ); },
    range.parameters()
  );
  ENDFtk::section::Type< 2, 151 > mt151{
    double( params.ZA() ), params.AWR(), range.EL(), range.EH(), SPI, AP
  };

  // MT152
  const auto& unresolved = data.unresolved();
  if( unresolved.empty() ){
    // There are no unresolved resonances
    return ENDFtk::file::Type< 2 >{ std::move( mt151 ) };
  }

  logger.first << "\tMT=152" << std::endl;
  const auto& proj = data.projectile();
  const auto& target = data.target();

  ReactionID tID{ proj, target, elementary::ReactionType{ "total" } };
  ReactionID eID{ proj, target, elementary::ReactionType{ "elastic" } };
  ReactionID fID{ proj, target, elementary::ReactionType{ "fission" } };
  ReactionID cID{ proj, target, elementary::ReactionType{ "capture" } };

  if( unresolved.count( eID ) == 0 ){
    Log::error( "Can't find unresolved elastic reaction." );
    throw std::exception();
  }
  auto eRxn = unresolved.at( eID );
  auto energies = eRxn.template crossSections< XSPair >().first;
  std::vector< double > total, elastic, capture, fission;

  auto getUXS = [&]( std::string name, auto&& ID ) -> std::vector< double >{
    try {
      logger.first << fmt::format( "\t{}: {:s}\n", name,  ID.symbol() );
      return unresolved.at( ID ).template crossSections< XSPair >().second;
    } catch( std::out_of_range& ){
      Log::error( "Unable to extract " + name + " unresolved cross section." );
      throw;
    }
  };

  total   = getUXS( "total", tID );
  elastic = getUXS( "elastic", eID );
  capture = getUXS( "capture", cID );

  if( unresolved.count( fID ) != 0 ){
    fission = getUXS( "fission", fID );
  } else {
    fission = std::vector< double >( energies.size(), 0.0 );
  }

  ENDFtk::section::Type< 2, 152 > mt152{
    static_cast< double >( eRxn.ZA() ), eRxn.AWR(), eRxn.LSSF(), 2, 0.0, 
    utility::copy( energies ),
    utility::copy( total ),
    utility::copy( elastic ),
    utility::copy( fission ),
    utility::copy( capture ),
    std::move( total )
  };

  return ENDFtk::file::Type< 2 >{ std::move( mt151 ), std::move( mt152 ) };
}
