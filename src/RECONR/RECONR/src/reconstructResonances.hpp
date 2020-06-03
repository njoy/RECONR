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
     R2D2::ReconMap_t& reconstructed,
     const ResonanceRange& resonanceRange,
     const SpecialCase&,
     double, double ){
  output << "Reconstructing scattering cross section for a SpecialCase."
         << std::endl;

  auto eL = resonanceRange.EL();
  auto eH = resonanceRange.EH();
  auto specialCase = std::get< ENDFtk::resonanceParameters::SpecialCase >( 
      resonanceRange.parameters() );

  auto AP = specialCase.scatteringRadius();

  // NJOY2016 uses the formula 4*pi*Ap*Ap in the csnorp subroutine
  double sigma = 2*constants::twopi*AP*AP;

  std::vector< double > E{ eL, eH };
  std::vector< double > barns( 2, sigma );

  reconstructed[ 2 ].push_back(
    interp::LinearLinear{ std::move( E ), std::move( barns ) }
  );

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
R2D2::ReconMap_t reconstructResonances( 
    std::ostream& output,
    Range& grid, 
    std::vector< ENDFtk::resonanceParameters::Isotope >& isotopes,
    double relTol, double absTol ){

  R2D2::ReconMap_t reconstructed{};

  for( const auto& iso : isotopes ){
    for( const auto& range : iso.resonanceRanges() ){
      std::visit(
        [&]( auto&& param ){ return reconstructResonances( 
              output, grid, reconstructed, range, param, relTol, absTol ); },
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
