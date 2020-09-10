namespace interp{

inline
std::vector< PPForms >
MF13toInterpolation( const ENDFtk::section::Type< 13 >& section ){

  std::vector< PPForms > xs;

  // I never use the total cross section in RECONR
  for( const auto& partial : section.partialCrossSections() ){
    xs.push_back( TAB1toInterpolation( partial, section.MT(), 0.0 ) );
  }
  return xs;
}

} // namespace interp
