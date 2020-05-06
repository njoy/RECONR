template< typename Range, typename P >
static
void reconstructResonances( Range& , 
                            R2D2::LinMMap_t& , 
                            const ResonanceRange&,
                            const P& params ){ 
  auto name = typeid( params ).name();

  Log::error( "I don't know how to reconstruct {} parameters", name );
  throw std::exception();
}

template< typename Range >
static
void reconstructResonances( 
    Range& grid, 
    R2D2::LinMMap_t& ,
    const ResonanceRange& rRange,
    const resolved::SingleLevelBreitWigner&
    ){
  Log::info( "reconstructing SLBW" );

  resonanceReconstruction::breitWigner::singleLevel::Apply{}(
      rRange, linearize( grid, 1E-3, 1E-7 ) );
}

template< typename Range >
static
R2D2::LinMMap_t reconstructResonances( Range& grid, 
    std::vector< ENDFtk::resonanceParameters::Isotope >& Isotopes ){

  R2D2::LinMMap_t reconstructed{};

  for( const auto& iso : Isotopes ){
    for( const auto& range : iso.resonanceRanges() ){
      std::visit(
        [&]( auto&& param ){ 
          return reconstructResonances( grid, reconstructed, range, param ); },
        range.parameters()
      );
    }
  }
  return reconstructed;
}

template< typename Range >
static
void reconstructResonances( Range& grid, 
                            ResonanceReconstructionDataDelivery& r2d2){
  auto g = grid | ranges::view::transform(
    []( auto&& arg ){ return arg*dimwits::Quantity< ElectronVolt >(); }
  );
  r2d2.reconstructedCrossSections( 
    std::visit( 
      [&]( auto&& arg ){ return reconstructResonances( grid, arg ); },
      r2d2.resonanceParameters()
    )
  );
}
