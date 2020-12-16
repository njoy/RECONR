template< typename U >
static
void reconstructUnresolved(
  const Logger&,
  R2D2& r2d2,
  const RP::ResonanceRange& rRange,
  const U& uRange,
  double relTol, double absTol ){

  const auto& MT451 = std::get< 0 >( r2d2.info() );
  const auto& nMass = CODATA[ constants::neutronMass ];
  const auto& eCharge = CODATA[ constants::elementaryCharge ];

  auto lru = resonanceReconstruction::rmatrix::fromENDF( 
    rRange, nMass, eCharge, r2d2.projectile(), r2d2.target() );

  auto energies = lru.grid();

  auto crossSections = energies 
    | ranges::view::transform( [&]( auto&& energy ){ return lru( energy ); } )
    | ranges::to_vector;

  auto x = energies 
    | ranges::view::transform( 
      []( auto&& energy ) -> double { return energy / dimwits::electronVolt; }
    );

  auto& unresolved = r2d2.unresolved();
  auto IDs = ranges::view::keys( crossSections.front() );
  for( auto& id : IDs ){

    auto xs = crossSections
      | ranges::view::transform( 
          [&]( auto&& m ) -> double { return m.at( id ) / dimwits::barns; } )
      | ranges::to_vector;

    auto INT = lru.interpolation().value_or( 5 );
    auto table = interp::makeInterpolationTable( 
        x, xs, 0, ranges::distance( x ), INT );
    auto linlin = std::visit(
      [&]( auto&& t ){ return linearize( t, relTol, absTol ); },
      table );

    URxn rxn{ MT451.ZA(), MT451.AWR(), uRange.LSSF(), std::move( linlin ) };
    unresolved.emplace( id , std::move( rxn ) );
  }
}
