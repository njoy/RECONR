ENDFtk::file::Type< 2 >
mf2( const int& MAT,
     const R2D2& data ){

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

  const auto& proj = data.projectile();
  const auto& target = data.target();

  ReactionID tID{ proj, target, elementary::ReactionType{ "total" } };
  ReactionID eID{ proj, target, elementary::ReactionType{ "elastic" } };
  ReactionID fID{ proj, target, elementary::ReactionType{ "fission" } };
  ReactionID cID{ proj, target, elementary::ReactionType{ "capture" } };

  
  auto eRxn = unresolved.at( eID );
  auto& energies = eRxn.template crossSections< XSPair >().first;

  std::vector< double > elastic = 
      eRxn.template crossSections< XSPair >().second;
  std::vector< double > total = 
      unresolved.at( tID ).template crossSections< XSPair >().second;
  std::vector< double > capture = 
      unresolved.at( cID ).template crossSections< XSPair >().second;
  std::vector< double > fission;
  if( unresolved.count( fID ) != 0 ){
    fission = unresolved.at( fID ).template crossSections< XSPair >().second;
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
