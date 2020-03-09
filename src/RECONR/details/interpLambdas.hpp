#ifndef RECONR_DETAILS_INTERPLAMBDAS_HPP
#define RECONR_DETAILS_INTERPLAMBDAS_HPP

auto histoInterpolation = []( double, 
                               double, double, 
                               double y1, double ){
  return y1;
};
auto linlinInterpolation = []( double x, 
                               double x1, double x2, 
                               double y1, double y2 ){
  return y1 + ( y2 - y1) * ( x - x1 ) / ( x2 - x1 );
};
auto linlogInterpolation = []( double x, 
                               double x1, double x2, 
                               double y1, double y2 ){
  const auto inverseX1 = 1.0 / x1;
  return y1 + ( y2 - y1 )*
    std::log( x*inverseX1 )/std::log( x2*inverseX1 );
};
auto loglinInterpolation = []( double x, 
                               double x1, double x2, 
                               double y1, double y2 ){
  return y1*std::pow( ( y2/y1 ), ( x - x1 )/( x2 - x1 ) );
};
auto loglogInterpolation = []( double x, 
                               double x1, double x2, 
                               double y1, double y2 ){
  const auto inverseX1 = 1.0 / x1;
  const auto logRatio = std::log( y2/y1 )/std::log( x2 * inverseX1 );
  return y1*std::pow( x*inverseX1, logRatio );
};
#endif // RECONR_DETAILS_INTERPLAMBDAS_HPP
