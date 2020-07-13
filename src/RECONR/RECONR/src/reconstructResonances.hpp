template< typename Range, typename P >
static
void reconstructResonances( std::ostream&,
                            Range& , 
                            R2D2::ReconMap_t& , 
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
     R2D2::ReconMap_t&,
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
     R2D2::ReconMap_t&,
     const ResonanceRange&,
     const unresolved::EnergyIndependent&,
     double, double){

  output << "Reconstructing unresolved EnergyIndependent parameters." 
         << std::endl;
}

template< typename Range >
static
void reconstructResonances( 
     std::ostream& output,
     Range&,
     R2D2::ReconMap_t&,
     const ResonanceRange&,
     const unresolved::EnergyDependentFissionWidths&,
     double, double ){

  output << "Reconstructing unresolved EnergyDependentFissionWidths parameters." 
         << std::endl;
}

template< typename Range >
static
void reconstructResonances( 
     std::ostream& output,
     Range&,
     R2D2::ReconMap_t&,
     const ResonanceRange&,
     const unresolved::EnergyDependent&,
     double, double ){

  output << "Reconstructing unresolved EnergyDependent parameters." 
         << std::endl;
}

template< typename Range >
static
void reconstructResonances( 
     std::ostream& output,
     Range& grid, 
     R2D2::ReconMap_t& reconstructed,
     const ResonanceRange& rRange,
     const resolved::SingleLevelBreitWigner&,
     double relTol, double absTol){

  output << "Reconstructing SLBW resonances." << std::endl;

  auto bw = resonanceReconstruction::breitWigner::singleLevel::Apply{}(
      rRange, linearize( grid, relTol, absTol ) );

  storeBWReconstruction( bw, reconstructed );

}

template< typename Range >
static
void reconstructResonances( 
     std::ostream& output,
     Range& grid, 
     R2D2::ReconMap_t& reconstructed,
     const ResonanceRange& rRange,
     const resolved::MultiLevelBreitWigner&,
     double relTol, double absTol){

  output << "Reconstructing SLBW resonances." << std::endl;

  auto bw = resonanceReconstruction::breitWigner::multiLevel::Apply{}(
      rRange, linearize( grid, relTol, absTol ) );

  storeBWReconstruction( bw, reconstructed );

}

template< typename Range >
static
void reconstructResonances( 
     std::ostream& output,
     Range& grid, 
     R2D2::ReconMap_t& reconstructed,
     const ResonanceRange& rRange,
     const resolved::ReichMoore&,
     double relTol, double absTol){

  output << "Reconstructing Reich-Moore resonances." << std::endl;

  auto bw = resonanceReconstruction::reichMoore::Apply{}(
      rRange, linearize( grid, relTol, absTol ) );

  storeBWReconstruction( bw, reconstructed );

}

template< typename Range >
static
void reconstructResonances( 
     std::ostream& output,
     Range& grid, 
     R2D2::ReconMap_t& reconstructed,
     const ResonanceRange& rRange,
     const resolved::RMatrixLimited&,
     double relTol, double absTol){

  output << "Reconstructing R-Matrix Limited resonances." << std::endl;

  auto nMass = CODATA[ constants::neutronMass ];
  auto eCharge = CODATA[ constants::elementaryCharge ];
  // auto nMass = 1.008664 * dimwits::daltons;
  // auto eCharge = 1.602e-19 * dimwits::coulomb;

  auto rml = resonanceReconstruction::rmatrix::fromENDF( 
    rRange, nMass, eCharge );

  auto [ energies, crossSections ] = linearize( grid, rml, relTol, absTol );
  // Remove units from x
  auto x = energies 
    | ranges::view::transform( 
      []( auto&& energy ) -> double { return energy / dimwits::electronVolt; }
    );

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
R2D2::ReconMap_t reconstructResonances( 
    std::ostream& output,
    Range& grid, 
    std::vector< ENDFtk::resonanceParameters::Isotope >& isotopes,
    double relTol, double absTol ){

  R2D2::ReconMap_t reconstructed{};

  for( const auto& iso : isotopes ){
    for( const auto& range : iso.resonanceRanges() ){
      auto eL = range.EL()*dimwits::electronVolt;
      auto eH = range.EH()*dimwits::electronVolt;

      auto g = grid 
        | ranges::view::filter( 
          [&]( auto&& e ){ return ( e >= eL ) and ( e <= eH ); } )
        | ranges::to_vector;

      std::visit(
        [&]( auto&& param ){ return reconstructResonances( 
              output, g, reconstructed, range, param, relTol, absTol ); },
        range.parameters()
      );
    }
  }
  return reconstructed;
}

template< typename Range >
static
R2D2::ReconMap_t reconstructResonances( 
    std::ostream& output,
    Range& grid, 
    ENDFtk::section::Type< 2, 151 >& parameters,
    double relTol, double absTol ){

  auto isotopes = parameters.isotopes() | ranges::to_vector;
  return reconstructResonances( output, grid, isotopes , relTol, absTol );
}


template< typename Range >
static
void reconstructResonances( std::ostream& output,
                            Range& grid, 
                            ResonanceReconstructionDataDelivery& r2d2,
                            double relTol, double absTol ){
  auto g = grid | ranges::view::transform(
    []( auto&& v )->dimwits::Quantity< dimwits::ElectronVolt >
      { return v*dimwits::electronVolt; }
  );

  r2d2.reconstructedResonances( 
    std::visit( 
      [&]( auto&& arg ){ 
        return reconstructResonances( output, g, arg, relTol, absTol ); },
      r2d2.resonanceParameters()
    )
  );
}
