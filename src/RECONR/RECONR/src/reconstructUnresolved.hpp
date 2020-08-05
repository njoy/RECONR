template< typename U >
static
void reconstructUnresolved(
  std::ostream& output,
  R2D2& r2d2,
  const ResonanceRange& rRange,
  const U& uRange ){

  const auto& MT451 = std::get< 0 >( r2d2.info() );
  const auto& nMass = CODATA[ constants::neutronMass ];
  const auto& eCharge = CODATA[ constants::elementaryCharge ];

  auto lru = resonanceReconstruction::rmatrix::fromENDF( 
    rRange, nMass, eCharge, r2d2.projectile(), r2d2.target() );

  const auto energies = lru.grid();
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

    URxn rxn{ MT451.ZA(), MT451.AWR(), uRange.LSSF(), 
              interp::LinearLinear{ x | ranges::to_vector, std::move( xs ) }
            };
    unresolved.emplace( id , std::move( rxn ) );
  }
}
