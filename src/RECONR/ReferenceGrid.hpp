namespace RP = ENDFtk::resonanceParameters;

template< typename T >
struct echo;

class ReferenceGrid {
private:
  mutable std::array<double, 3> buffer;

public:
  #include "RECONR/ReferenceGrid/src/call.hpp"
};
