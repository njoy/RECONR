ENDFtk::file::Type< 2 >
mf2( const int& MAT,
     const R2D2& data ){

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

  return ENDFtk::file::Type< 2 >{ std::move( mt151 ) };
}
