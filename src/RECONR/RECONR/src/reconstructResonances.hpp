template< typename Range, typename P >
static
void reconstructResonances( std::ostream&,
                            Range& , 
                            R2D2&,
                            const RP::ResonanceRange&,
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
     const RP::ResonanceRange&,
     const RP::SpecialCase&,
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
  const RP::ResonanceRange& rRange,
  const RP::CaseA& uRange,
  double relTol, double absTol){

  logger.first << "Reconstructing unresolved EnergyIndependent parameters." 
         << std::endl;
  reconstructUnresolved( logger, r2d2, rRange, uRange, relTol, absTol );
}

template< typename Range >
static
void reconstructResonances( 
  const Logger& logger,
  Range&,
  R2D2& r2d2,
  const RP::ResonanceRange& rRange,
  const RP::CaseB& uRange,
  double relTol, double absTol){

  logger.first 
    << "Reconstructing unresolved EnergyDependentFissionWidths parameters." 
         << std::endl;
  reconstructUnresolved( logger, r2d2, rRange, uRange, relTol, absTol );
}

template< typename Range >
static
void reconstructResonances( 
  const Logger& logger,
  Range&,
  R2D2& r2d2,
  const RP::ResonanceRange& rRange,
  const RP::CaseC& uRange,
  double relTol, double absTol){

  logger.first << "Reconstructing unresolved parameters." 
         << std::endl;

  reconstructUnresolved( logger, r2d2, rRange, uRange, relTol, absTol );
}

template< typename Range >
static
void reconstructResonances( 
  const Logger& logger,
  Range& grid, 
  R2D2& r2d2, 
  const RP::ResonanceRange& rRange,
  const RP::SLBW&,
  double relTol, double absTol){

  logger.first << "Reconstructing SLBW resonances." << std::endl;

  auto bw = resonanceReconstruction::breitWigner::singleLevel::Apply{}(
      rRange, linearizeRes( grid, relTol, absTol ) );

  storeBWReconstruction( bw, r2d2 );

}

template< typename Range >
static
void reconstructResonances( 
     const Logger& logger,
     Range& grid, 
     R2D2& r2d2, 
     const RP::ResonanceRange& rRange,
     const RP::MLBW&,
     double relTol, double absTol){

  logger.first << "Reconstructing MLBW resonances." << std::endl;

  auto bw = resonanceReconstruction::breitWigner::multiLevel::Apply{}(
      rRange, linearizeRes( grid, relTol, absTol ) );

  storeBWReconstruction( bw, r2d2 );

}

template< typename Range >
static
void reconstructResonances( 
     const Logger& logger,
     Range& grid, 
     R2D2& r2d2,
     const RP::ResonanceRange& rRange,
     const RP::RM&,
     double relTol, double absTol){

  logger.first << "Reconstructing Reich-Moore resonances." << std::endl;

  auto bw = resonanceReconstruction::reichMoore::Apply{}(
      rRange, linearizeRes( grid, relTol, absTol ) );

  storeBWReconstruction( bw, r2d2 );

}

template< typename Range >
static
void reconstructResonances( 
     const Logger& logger,
     Range& grid, 
     R2D2& r2d2,
     const RP::ResonanceRange& rRange,
     const RP::RML&,
     double relTol, double absTol){

  logger.first << "Reconstructing R-Matrix Limited resonances." << std::endl;

  const auto& nMass = CODATA[ constants::neutronMass ];
  const auto& eCharge = CODATA[ constants::elementaryCharge ];

  auto rml = resonanceReconstruction::rmatrix::fromENDF( 
    rRange, nMass, eCharge, r2d2.projectile(), r2d2.target() );

  auto [ energies, crossSections ] = linearize( grid, rml, relTol, absTol );
  // Remove units from x
  auto x = energies 
    | ranges::view::transform( 
      []( auto&& energy ) -> double { return energy / dimwits::electronVolt; }
    );

  auto& reconstructed = r2d2.reconstructedResonances();
  auto IDs = ranges::view::keys( crossSections.front() );
  for( auto& id : IDs ){

    auto xs = crossSections
      | ranges::view::transform( 
          [&]( auto&& m ) -> double { return m.at( id ) / dimwits::barns; } )
      | ranges::to_vector;
    if( elementary::toEndfReactionNumber( id ) == 601 ){
    }

    reconstructed[ id ].push_back(
      interp::LinearLinear{ x | ranges::to_vector, std::move( xs ) } );
  }

}

template< typename Range >
static
void reconstructResonances( 
    const Logger& logger,
    Range& grid, 
    R2D2& r2d2,
    std::vector< RP::Isotope >& isotopes,
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
