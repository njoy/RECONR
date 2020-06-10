
inline
std::vector< std::vector< Variant > >
MF13toInterpolation( const ENDFtk::section::Type< 13 >& section ){
  std::vector< std::vector< Variant > > xs;

  // I never use the total cross section in RECONR
  for( const auto& partial : section.partialCrossSections() ){
    xs.push_back( TAB1toInterpolation( partial ) );
  }
  return xs;
}
