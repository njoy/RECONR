template< typename Range, typename P >
static
void reconstructResonances( std::ostream&,
                            Range& , 
                            R2D2&,
                            const ResonanceRange&,
                            const P& params,
                            double, double ){ 
  auto name = typeid( params ).name();

  Log::error( "I don't know how to reconstruct {} parameters", name );
  throw std::exception();
}

template< typename Range >
static
void reconstructResonances( 
     const Logger& logger,
     Range&,
     R2D2&,
     const ResonanceRange&,
     const SpecialCase&,
     double, double ){
  logger.first << "No resonance reconstruction needed for a SpecialCase." 
         << std::endl;
  // Nothing to do for SpecialCase
}

template< typename Range >
static
void reconstructResonances( 
  const Logger& logger,
  Range&,
  R2D2& r2d2,
  const ResonanceRange& rRange,
  const unresolved::EnergyIndependent& uRange,
  double, double){

  logger.first << "Reconstructing unresolved EnergyIndependent parameters." 
         << std::endl;
  reconstructUnresolved( logger, r2d2, rRange, uRange );
}

template< typename Range >
static
void reconstructResonances( 
  const Logger& logger,
  Range&,
  R2D2& r2d2,
  const ResonanceRange& rRange,
  const unresolved::EnergyDependentFissionWidths& uRange,
  double, double ){

  logger.first 
    << "Reconstructing unresolved EnergyDependentFissionWidths parameters." 
         << std::endl;
  reconstructUnresolved( logger, r2d2, rRange, uRange );
}

template< typename Range >
static
void reconstructResonances( 
  const Logger& logger,
  Range&,
  R2D2& r2d2,
  const ResonanceRange& rRange,
  const unresolved::EnergyDependent& uRange,
  double, double ){

  logger.first << "Reconstructing unresolved parameters." 
         << std::endl;

  reconstructUnresolved( logger, r2d2, rRange, uRange );
}

template< typename Range >
static
void reconstructResonances( 
  const Logger& logger,
  Range& grid, 
  R2D2& r2d2, 
  const ResonanceRange& rRange,
  const resolved::SingleLevelBreitWigner&,
  double relTol, double absTol){

  logger.first << "Reconstructing SLBW resonances." << std::endl;

  reconstructViaReconstructor( logger, grid, r2d2, rRange, relTol, absTol );

}

template< typename Range >
static
void reconstructResonances( 
     const Logger& logger,
     Range& grid, 
     R2D2& r2d2, 
     const ResonanceRange& rRange,
     const resolved::MultiLevelBreitWigner&,
     double relTol, double absTol){

  logger.first << "Reconstructing SLBW resonances." << std::endl;

  reconstructViaReconstructor( logger, grid, r2d2, rRange, relTol, absTol );

}

template< typename Range >
static
void reconstructResonances( 
     const Logger& logger,
     Range& grid, 
     R2D2& r2d2,
     const ResonanceRange& rRange,
     const resolved::ReichMoore&,
     double relTol, double absTol){

  logger.first << "Reconstructing Reich-Moore resonances." << std::endl;

  reconstructViaReconstructor( logger, grid, r2d2, rRange, relTol, absTol );

}

template< typename Range >
static
void reconstructResonances( 
     const Logger& logger,
     Range& grid, 
     R2D2& r2d2,
     const ResonanceRange& rRange,
     const resolved::RMatrixLimited&,
     double relTol, double absTol){

  logger.first << "Reconstructing R-Matrix Limited resonances." << std::endl;

  reconstructViaReconstructor( logger, grid, r2d2, rRange, relTol, absTol );

}

template< typename Range >
static
void reconstructResonances( 
    const Logger& logger,
    Range& grid, 
    R2D2& r2d2,
    std::vector< ENDFtk::resonanceParameters::Isotope >& isotopes,
    double relTol, double absTol ){


  for( const auto& iso : isotopes ){
    for( const auto& range : iso.resonanceRanges() ){
      auto eL = range.EL()*dimwits::electronVolt;
      auto eH = range.EH()*dimwits::electronVolt;

      auto g = grid 
        | ranges::view::filter( 
          [&]( auto&& e ){ return ( e >= eL ) and ( e <= eH ); } )
        | ranges::to_vector;

      std::visit(
        [&]( auto&& param ){ return RECONR::reconstructResonances( 
              logger, g, r2d2, range, param, relTol, absTol ); },
        range.parameters()
      );
    }
  }
}

template< typename Range >
static
void reconstructResonances( 
    const Logger& logger,
    Range& grid, 
    R2D2& r2d2,
    const ENDFtk::section::Type< 2, 151 >& parameters,
    double relTol, double absTol ){

  auto isotopes = parameters.isotopes() | ranges::to_vector;
  RECONR::reconstructResonances( 
      logger, grid, r2d2, isotopes , relTol, absTol );
}


template< typename Range >
static
void reconstructResonances( const Logger& logger,
                            Range& grid, 
                            ResonanceReconstructionDataDelivery& r2d2,
                            double relTol, double absTol ){
  auto g = grid | ranges::view::transform(
    []( auto&& v )->dimwits::Quantity< dimwits::ElectronVolt >
      { return v*dimwits::electronVolt; })
    | ranges::to_vector;

  std::visit( 
    [&]( auto&& arg ){ 
      RECONR::reconstructResonances( logger, g, r2d2, arg, relTol, absTol ); },
    r2d2.resonanceParameters()
  );
}
