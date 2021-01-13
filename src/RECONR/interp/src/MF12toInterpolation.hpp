namespace interp{

inline
std::vector< PYForms >
MF12toInterpolation( const ENDFtk::section::Type< 12 >& section ){

  std::vector< PYForms > xs;

  // I never use the total in RECONR
  for( const auto& partial : 
       std::get< 0 >( section.photonProduction() ).partialMultiplicities() ){
    xs.push_back( TAB1toInterpolation( partial, section.MT(), 0.0 ) );
  }
  return xs;
}

} // namespace interp

