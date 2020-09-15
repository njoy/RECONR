template< typename Range >
std::array<double, 3>
operator()( 
  const ENDFtk::resonanceParameters::resolved::BreitWignerLValue::Resonance<
    Range >& resonance ) const {
  const double energy = resonance.ER();
  const double halfWidth = 0.5 * resonance.GT();
  return {{ energy - halfWidth, energy, energy + halfWidth }};
}

template< typename Range >
std::array<double, 3>
operator()( 
  const ENDFtk::resonanceParameters::resolved::ReichMooreLValue::Resonance<
           Range >& resonance ) const {
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
                 const ResonanceRange& rRange,
                 const elementary::ParticleID&,
                 const elementary::ParticleID& ) const {
  auto lowerEnergy = sigfig( rRange.EL(), -1E-7 );
  auto upperEnergy = sigfig( rRange.EH(), 1E-7 );

  std::vector< double > energies;
  energies.reserve( 12 * range.lValues().size() );

  energies.push_back( lowerEnergy );

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

  energies.push_back( upperEnergy );

  if ( not ranges::is_sorted( energies ) ){
    std::sort( energies.begin(), energies.end() );
  }

  energies |= ranges::action::unique;
  return energies;
}

auto operator()( const resolved::RMatrixLimited& rml,
                 const ResonanceRange& rRange,
                 const elementary::ParticleID& target,
                 const elementary::ParticleID& proj ) const {

  auto eV = dimwits::electronVolt;
  auto lowerEnergy = sigfig( rRange.EL(), -1E-7 );
  auto upperEnergy = sigfig( rRange.EH(), 1E-7 );

  const auto& nMass = CODATA[ constants::neutronMass ];
  const auto& eCharge = CODATA[ constants::elementaryCharge ];

  auto grid = resonanceReconstruction::rmatrix::fromENDF( 
    rRange, nMass, eCharge, proj, target ).grid();

  std::vector< double > energies;

  energies.push_back( lowerEnergy );
  ranges::action::push_back( energies, 
    grid
    | ranges::view::transform( 
        [&]( auto&& energy ){ return energy/eV; } )
  );

  energies.push_back( upperEnergy );

  if ( not ranges::is_sorted( energies ) ){
    std::sort( energies.begin(), energies.end() );
  }

  energies |= ranges::action::unique;
  return energies;
};

auto operator()( const unresolved::CaseA&,
                 const ResonanceRange& rRange,
                 const elementary::ParticleID&,
                 const elementary::ParticleID& ) const {
  auto lowerEnergy = sigfig( rRange.EL(), -1E-7 );
  auto upperEnergy = sigfig( rRange.EH(), 1E-7 );

  std::vector< double > energies;
  /*
   * log( EH / EL ) / log( 10^(1/13) )
   * = 13 * log( EH / EL ) / log( 10 )
   * = 5.645828264742274 * log( EH / EL )
   */
  energies.reserve( std::ceil( 5.645828264742274 *
                               std::log( upperEnergy / lowerEnergy ) ) );
  energies.push_back( lowerEnergy );
  fill( lowerEnergy, upperEnergy, energies );
  energies.push_back( upperEnergy );
  return energies;
}

auto operator()( const unresolved::CaseB& caseB,
                 const ResonanceRange& rRange,
                 const elementary::ParticleID&,
                 const elementary::ParticleID& ) const {
  auto lowerEnergy = sigfig( rRange.EL(), -1E-7 );
  auto upperEnergy = sigfig( rRange.EH(), 1E-7 );

  std::vector< double > energies;
  energies.reserve( 8 * std::ceil( std::log( upperEnergy / lowerEnergy ) ) );
  energies.push_back( lowerEnergy );
  auto clampedES =
    caseB.ES()
    | ranges::view::drop_while
      ( [=]( auto energy ){ return energy <= lowerEnergy; } )
    | ranges::view::take_while
      ( [=]( auto energy ){ return energy < upperEnergy; } );

  RANGES_FOR( const auto energy, clampedES ){
    fill( energies.back(), energy, energies );
  }

  fill( energies.back(), upperEnergy, energies );
  energies.push_back( upperEnergy );
  return energies;
}

template< typename UnresolvedLvalue,
          if_< hasJvalues< UnresolvedLvalue > > = true >
auto
operator()( const UnresolvedLvalue& lValue ) const {
  return
    lValue.jValues()
    | ranges::view::transform
      ( []( const auto& jValue ){ return jValue.ES(); } );
}

auto operator()( const unresolved::CaseC& caseC,
                 const ResonanceRange& rRange,
                 const elementary::ParticleID&,
                 const elementary::ParticleID& ) const {
  std::vector< double > energies;

  /* Case C is composed of Lvalues which are in turn composed of Jvalues 
   * which in turn composed of a tuple of region averaged resonance parameter 
   * values. Each such tuple has an energy element. This energy value may
   * actually fall outside the energy range of the unresolved region.
   *
   * In order to recover a reference grid for reconstruction, we make two passes
   * over this collection of energies.
   *
   * In the first pass, we begin by extracting all energy values within the
   * range of the unresolved region, sorting them, and removing duplicates.
   */
  auto lowerEnergy = sigfig( rRange.EL(), -1E-7 );
  auto upperEnergy = sigfig( rRange.EH(), 1E-7 );

  std::vector< double > firstPass;
  firstPass.push_back( lowerEnergy );
  {
    auto jEnergies = caseC.lValues() | ranges::view::for_each( *this );
    RANGES_FOR( const auto jEnergy, jEnergies ){
      ranges::action::push_back( firstPass,
                                 jEnergy
                                 | ranges::view::filter
                                   ( [=]( const auto energy )
                                     { return lowerEnergy < energy
                                              and energy < upperEnergy; } ) );
    }
  }

  firstPass.push_back( upperEnergy );
  std::sort( firstPass.begin(), firstPass.end() );
  // orlp::pdqsort( firstPass.begin(), firstPass.end() );
  ranges::action::unique( firstPass );

  /* In the second pass, beginning with the first pass grid, we insert points 
   * such that the reference grid meets some minimum density requirements.
   */
  energies.reserve( 8 * firstPass.size() );
  energies.push_back( firstPass.front() );

  for( const auto energy : firstPass | ranges::view::drop_exactly(1) ){
    fill( energies.back(), energy, energies );
  }
  energies.push_back( upperEnergy );

  return energies;
}

std::vector< double >
operator()( const SpecialCase&,
            const ResonanceRange& rRange,
            const elementary::ParticleID&,
            const elementary::ParticleID& ) const {
  return { sigfig( rRange.EL(), -1E-7 ), sigfig( rRange.EH(), 1E-7 ) };

}

template< typename... TS >
auto operator()( const std::variant< TS... >& range_variant, 
                 const ResonanceRange& rRange,
                 const elementary::ParticleID& target,
                 const elementary::ParticleID& proj ) const {
  return std::visit( 
    [&]( auto&& arg ){ return (*this)( arg, rRange, target, proj ); },
    range_variant
  );
}

auto operator()( const Isotope& isotope,
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
  return grids | ranges::view::join | ranges::to_vector;
}
