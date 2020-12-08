static
std::pair< std::optional< Range_t >, std::optional< Range_t > >
collectResonanceRanges( const ENDFtk::section::Type< 2, 151 >& mt151 ){

  // These are the same defaults that Legacy NJOY uses
  // Range_t resolved{ 1E10, 0.0 };
  // Range_t unresolved{ 1E10, 0.0 };
  std::optional< Range_t > resolved;
  std::optional< Range_t > unresolved;

  for( const auto& isotope : mt151.isotopes() ){
    for( const auto& range : isotope.resonanceRanges() ){
      switch ( range.LRU() ) {
        case 0:
          // Do nothing
          break;
        case 1:
          if( resolved ){
            resolved = Range_t{
              std::min( resolved->first, range.lowerEnergy() ),
              std::max( resolved->second, range.upperEnergy() )
            };
          } else {
            resolved = Range_t{ range.lowerEnergy(), range.upperEnergy() };
          }
          break;
        case 2:
          if( unresolved ){
            unresolved = Range_t{
              std::min( unresolved->first, range.lowerEnergy() ),
              std::max( unresolved->second, range.upperEnergy() )
            };
          } else {
            unresolved = Range_t{ range.lowerEnergy(), range.upperEnergy() };
          }
          break;
        default:
          // A la @whaeck
          throw std::runtime_error( 
            fmt::format("You somehow reached unreachable code, LRU={}"
            "while collecting resonance ranges.", range.LRU() ) );
      } // switch
    } // for resonanceRanges
  } // for isotopes

  return std::make_pair( resolved, unresolved );
}

static
auto collectResonanceRanges( const RPVariant& params ){
  return std::visit( 
    [&]( auto&& arg ){ return Factory::collectResonanceRanges( arg ); },
    params );
}
