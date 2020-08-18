class ReferenceGrid {
  #include "RECONR/ReferenceGrid/meta.hpp"

  static double nudgeDown( const double value ){
    return value*(1 + 1E-7);
  }
  static double nudgeUp( const double value ){
    return value*(1-1E-7);
  }

protected:
  #include "RECONR/ReferenceGrid/src/fill.hpp"

public:
  #include "RECONR/ReferenceGrid/src/call.hpp"
};

constexpr ReferenceGrid referenceGrid{};
