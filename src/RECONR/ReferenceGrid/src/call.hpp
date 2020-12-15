template< typename Range >
std::array<double, 3>
operator()( 
  const RP::BreitWignerLValue::Resonance< Range >& resonance ) const {
  const double energy = resonance.ER();
  const double halfWidth = 0.5 * resonance.GT();
  return {{ energy - halfWidth, energy, energy + halfWidth }};
}

template< typename Range >
std::array<double, 3>
operator()( 
  const RP::ReichMooreLValue::Resonance< Range >& resonance ) const {
  const double energy = resonance.ER();
  const double halfWidth = 0.5 * ( resonance.GN()
                                   + resonance.GG()
                                   + resonance.GFA()
                                   + resonance.GFB() );
  return { { energy - halfWidth, energy, energy + halfWidth } };
}

template< typename ResolvedLvalue,
          if_< hasResonances< ResolvedLvalue > > = true >
auto
operator()( const ResolvedLvalue& lValue ) const {
  return lValue.resonances() | ranges::view::transform( *this );
}

// Resolved resolved resonance ranges
template< typename Range, if_< hasLvalues< Range > > = true >
auto operator()( const Range& range,
                 const RP::ResonanceRange& rRange,
                 const elementary::ParticleID&,
                 const elementary::ParticleID& ) const {
  auto lowerEnergy = rRange.EL();
  auto upperEnergy = rRange.EH();

  std::vector< double > energies;
  energies.reserve( 12 * range.lValues().size() );

  auto resonances = range.lValues() | ranges::view::for_each( *this );
  RANGES_FOR( auto resonance, resonances ){
    ranges::action::push_back( energies,
      resonance
      | ranges::view::filter
        ( [=]( const auto energy )
          { return lowerEnergy < energy and energy < upperEnergy; } 
        ) 
    );
  }
  return energies;
}

auto operator()( const RP::RML&,
                 const RP::ResonanceRange& rRange,
                 const elementary::ParticleID& target,
                 const elementary::ParticleID& proj ) const {

  auto eV = dimwits::electronVolt;

  const auto& nMass = CODATA[ constants::neutronMass ];
  const auto& eCharge = CODATA[ constants::elementaryCharge ];

  auto grid = resonanceReconstruction::rmatrix::fromENDF( 
    rRange, nMass, eCharge, proj, target ).grid();

  std::vector< double > energies;

  ranges::action::push_back( energies, 
    grid
    | ranges::view::transform( 
        [&]( auto&& energy ){ return energy/eV; } )
  );

  return energies;
}

auto operator()( const RP::CaseA&,
                 const RP::ResonanceRange& rRange,
                 const elementary::ParticleID& target,
                 const elementary::ParticleID& proj ) const {

  auto eV = dimwits::electronVolt;

  const auto& nMass = CODATA[ constants::neutronMass ];
  const auto& eCharge = CODATA[ constants::elementaryCharge ];

  auto grid = resonanceReconstruction::rmatrix::fromENDF( 
    rRange, nMass, eCharge, proj, target ).grid();

  std::vector< double > energies;

  ranges::action::push_back( energies, 
    grid
    | ranges::view::transform( 
        [&]( auto&& energy ){ return energy/eV; } )
  );

  return energies;
}

auto operator()( const RP::CaseB& caseB,
                 const RP::ResonanceRange& rRange,
                 const elementary::ParticleID& target,
                 const elementary::ParticleID& proj ) const {

  auto eV = dimwits::electronVolt;

  const auto& nMass = CODATA[ constants::neutronMass ];
  const auto& eCharge = CODATA[ constants::elementaryCharge ];

  auto grid = resonanceReconstruction::rmatrix::fromENDF( 
    rRange, nMass, eCharge, proj, target ).grid();

  std::vector< double > energies;

  ranges::action::push_back( energies, 
    grid
    | ranges::view::transform( 
        [&]( auto&& energy ){ return energy/eV; } )
  );

  return energies;
}

auto operator()( const RP::CaseC& caseC,
                 const RP::ResonanceRange& rRange,
                 const elementary::ParticleID& target,
                 const elementary::ParticleID& proj ) const {

  auto eV = dimwits::electronVolt;

  const auto& nMass = CODATA[ constants::neutronMass ];
  const auto& eCharge = CODATA[ constants::elementaryCharge ];

  auto grid = resonanceReconstruction::rmatrix::fromENDF( 
    rRange, nMass, eCharge, proj, target ).grid();

  std::vector< double > energies;

  ranges::action::push_back( energies, 
    grid
    | ranges::view::transform( 
        [&]( auto&& energy ){ return energy/eV; } )
  );

  return energies;
}

std::vector< double >
operator()( const RP::SpecialCase&,
            const RP::ResonanceRange&,
            const elementary::ParticleID&,
            const elementary::ParticleID& ) const {
  return { };

}

template< typename... TS >
std::vector< double >
operator()( const std::variant< TS... >& range_variant, 
            const RP::ResonanceRange& rRange,
            const elementary::ParticleID& target,
            const elementary::ParticleID& proj ) const {

  auto lowerEnergy = rRange.EL();
  auto upperEnergy = rRange.EH();

  auto values = std::visit( 
    [&]( auto&& arg ){ 
      return (*this)( arg, rRange, target, proj ); 
    },
    range_variant
  );

  auto energies = values
    | ranges::view::filter(
      [&]( const auto& energy ){  
        return lowerEnergy < energy and energy < upperEnergy; } 
  ) | ranges::to_vector;

  energies.insert( energies.begin(), lowerEnergy );
  energies.insert( energies.end(), upperEnergy );

  std::sort( energies.begin(), energies.end() );
  energies |= ranges::action::unique;

  return energies;
}

auto operator()( const RP::Isotope& isotope,
                 const elementary::ParticleID& target,
                 const elementary::ParticleID& proj ) const {
  auto grids =
    isotope.resonanceRanges()
    | ranges::view::transform( 
        [&]( auto&& range ){ 
          return (*this)( range.parameters(), range, target, proj ); 
        }
      )
    | ranges::to_vector;

  auto overlappingRegions =
    grids
    | ranges::view::sliding(2)
    | ranges::view::filter
      ( []( auto pair ){ return pair[0].back() >= pair[1].front(); } );

  bool foundOverlap = false;

  RANGES_FOR( auto pair, overlappingRegions ){
    auto& lowerGrid = pair[0];
    const auto boundary =
      std::find_if( lowerGrid.rbegin(), lowerGrid.rend(),
                    [ upperLimit = pair[1].front() ]
                    ( const auto energy )
                    { return energy <= upperLimit; } ).base();
    lowerGrid.erase( boundary, lowerGrid.end() );
    foundOverlap = true;
  }

  if ( foundOverlap ){
    Log::warning( "ENDF material has overlapping resonance ranges" );
  }

  grids |= ranges::action::unique;
  return grids 
    | ranges::view::join 
    | ranges::view::transform( 
      []( auto&& e ){ return utility::sigfig( e, 9, 0 ); } )
    | ranges::to_vector;
  // grids |= ranges::action::unique;
  // auto energies = grids | ranges::view::join | ranges::to_vector;
  // auto last = energies.back(); energies.pop_back();
  // energies.push_back( utility::sigfig( last, 7, -1 ) );
  // energies.push_back( utility::sigfig( last, 7, +1 ) );

  // return energies;
}
