template< typename Range >
static
void reconstructViaReconstructor(
     const Logger& logger,
     Range& grid, 
     R2D2& r2d2,
     const ResonanceRange& rRange,
     double relTol, double absTol){

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
