class ReferenceGrid {
  #include "RECONR/ReferenceGrid/meta.hpp"

  static double nudgeDown( const double value ){
    return std::nextafter( value, -std::numeric_limits< double >::infinity() );
  }

protected:
  #include "RECONR/ReferenceGrid/src/fill.hpp"

public:
  #include "RECONR/ReferenceGrid/src/call.hpp"
};

constexpr ReferenceGrid referenceGrid{};
