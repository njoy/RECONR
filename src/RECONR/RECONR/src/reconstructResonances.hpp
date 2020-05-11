template< typename Range, typename P >
static
void reconstructResonances( Range& , 
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
    Range& grid, 
    R2D2::ReconMap_t& reconstructed,
    const ResonanceRange& rRange,
    const resolved::SingleLevelBreitWigner&,
    double relTol, double absTol){

  auto bw = resonanceReconstruction::breitWigner::singleLevel::Apply{}(
      rRange, linearize( grid, relTol, absTol ) );

  storeBWReconstruction( bw, reconstructed );

}

template< typename Range >
static
void reconstructResonances( 
    Range& grid, 
    R2D2::ReconMap_t& reconstructed,
    const ResonanceRange& rRange,
    const resolved::MultiLevelBreitWigner&,
    double relTol, double absTol){

  auto bw = resonanceReconstruction::breitWigner::multiLevel::Apply{}(
      rRange, linearize( grid, relTol, absTol ) );

  storeBWReconstruction( bw, reconstructed );

}

template< typename Range >
static
void reconstructResonances( 
    Range& grid, 
    R2D2::ReconMap_t& reconstructed,
    const ResonanceRange& rRange,
    const resolved::ReichMoore&,
    double relTol, double absTol){

  auto bw = resonanceReconstruction::reichMoore::Apply{}(
      rRange, linearize( grid, relTol, absTol ) );

  storeBWReconstruction( bw, reconstructed );

}

template< typename Range >
static
R2D2::ReconMap_t reconstructResonances( Range& grid, 
    std::vector< ENDFtk::resonanceParameters::Isotope >& Isotopes,
    double relTol, double absTol ){

  R2D2::ReconMap_t reconstructed{};

  for( const auto& iso : Isotopes ){
    for( const auto& range : iso.resonanceRanges() ){
      std::visit(
        [&]( auto&& param ){ return reconstructResonances( 
              grid, reconstructed, range, param, relTol, absTol ); },
        range.parameters()
      );
    }
  }
  return reconstructed;
}

template< typename Range >
static
void reconstructResonances( Range& grid, 
                            ResonanceReconstructionDataDelivery& r2d2,
                            double relTol, double absTol ){
  auto g = grid | ranges::view::transform(
    []( auto&& v )->dimwits::Quantity< dimwits::ElectronVolt >
      { return v*dimwits::electronVolt; }
  );

  r2d2.reconstructedResonances( 
    std::visit( 
      [&]( auto&& arg ){ 
        return reconstructResonances( g, arg, relTol, absTol ); },
      r2d2.resonanceParameters()
    )
  );
}
