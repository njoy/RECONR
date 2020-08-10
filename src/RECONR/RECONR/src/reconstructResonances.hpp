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
     std::ostream& output,
     Range&,
     R2D2&,
     const ResonanceRange&,
     const SpecialCase&,
     double, double ){
  output << "No resonance reconstruction needed for a SpecialCase." 
         << std::endl;
  // Nothing to do for SpecialCase
}

template< typename Range >
static
void reconstructResonances( 
  std::ostream& output,
  Range&,
  R2D2& r2d2,
  const ResonanceRange& rRange,
  const unresolved::EnergyIndependent& uRange,
  double, double){

  output << "Reconstructing unresolved EnergyIndependent parameters." 
         << std::endl;
  reconstructUnresolved( output, r2d2, rRange, uRange );
}

template< typename Range >
static
void reconstructResonances( 
  std::ostream& output,
  Range&,
  R2D2& r2d2,
  const ResonanceRange& rRange,
  const unresolved::EnergyDependentFissionWidths& uRange,
  double, double ){

  output << "Reconstructing unresolved EnergyDependentFissionWidths parameters." 
         << std::endl;
  reconstructUnresolved( output, r2d2, rRange, uRange );
}

template< typename Range >
static
void reconstructResonances( 
  std::ostream& output,
  Range&,
  R2D2& r2d2,
  const ResonanceRange& rRange,
  const unresolved::EnergyDependent& uRange,
  double, double ){

  output << "Reconstructing unresolved parameters." 
         << std::endl;

  reconstructUnresolved( output, r2d2, rRange, uRange );
}

template< typename Range >
static
void reconstructResonances( 
  std::ostream& output,
  Range& grid, 
  R2D2& r2d2, 
  const ResonanceRange& rRange,
  const resolved::SingleLevelBreitWigner&,
  double relTol, double absTol){

  output << "Reconstructing SLBW resonances." << std::endl;

  auto bw = resonanceReconstruction::breitWigner::singleLevel::Apply{}(
      rRange, linearize( grid, relTol, absTol ) );

  storeBWReconstruction( bw, r2d2 );

}

template< typename Range >
static
void reconstructResonances( 
     std::ostream& output,
     Range& grid, 
     R2D2& r2d2, 
     const ResonanceRange& rRange,
     const resolved::MultiLevelBreitWigner&,
     double relTol, double absTol){

  output << "Reconstructing SLBW resonances." << std::endl;

  auto bw = resonanceReconstruction::breitWigner::multiLevel::Apply{}(
      rRange, linearize( grid, relTol, absTol ) );

  storeBWReconstruction( bw, r2d2 );

}

template< typename Range >
static
void reconstructResonances( 
     std::ostream& output,
     Range& grid, 
     R2D2& r2d2,
     const ResonanceRange& rRange,
     const resolved::ReichMoore&,
     double relTol, double absTol){

  output << "Reconstructing Reich-Moore resonances." << std::endl;

  auto bw = resonanceReconstruction::reichMoore::Apply{}(
      rRange, linearize( grid, relTol, absTol ) );

  storeBWReconstruction( bw, r2d2 );

}

template< typename Range >
static
void reconstructResonances( 
     std::ostream& output,
     Range& grid, 
     R2D2& r2d2,
     const ResonanceRange& rRange,
     const resolved::RMatrixLimited&,
     double relTol, double absTol){

  output << "Reconstructing R-Matrix Limited resonances." << std::endl;

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

    reconstructed[ id ].push_back(
      interp::LinearLinear{ x | ranges::to_vector, std::move( xs ) } );
  }

}

template< typename Range >
static
void reconstructResonances( 
    std::ostream& output,
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
              output, g, r2d2, range, param, relTol, absTol ); },
        range.parameters()
      );
    }
  }
}

template< typename Range >
static
void reconstructResonances( 
    std::ostream& output,
    Range& grid, 
    R2D2& r2d2,
    const ENDFtk::section::Type< 2, 151 >& parameters,
    double relTol, double absTol ){

  auto isotopes = parameters.isotopes() | ranges::to_vector;
  RECONR::reconstructResonances( output, grid, r2d2, isotopes , relTol, absTol );
}


template< typename Range >
static
void reconstructResonances( std::ostream& output,
                            Range& grid, 
                            ResonanceReconstructionDataDelivery& r2d2,
                            double relTol, double absTol ){
  auto g = grid | ranges::view::transform(
    []( auto&& v )->dimwits::Quantity< dimwits::ElectronVolt >
      { return v*dimwits::electronVolt; } )
    | ranges::to_vector;

  std::visit( 
    [&]( auto&& arg ){ 
      RECONR::reconstructResonances( output, g, r2d2, arg, relTol, absTol ); },
    r2d2.resonanceParameters()
  );
}
