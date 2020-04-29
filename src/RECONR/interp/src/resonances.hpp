namespace resonances {

#include "RECONR/interp/src/resonances/SLBW.hpp"
#include "RECONR/interp/src/resonances/MLBW.hpp"
#include "RECONR/interp/src/resonances/RM.hpp"

using Variant = std::variant<
  SLBW< double >,
  SLBW< double, double >,
  MLBW< double >,
  MLBW< double, double >,
  RM< double >,
  RM< double, double >
>;

using Vector = interpolation::Table< interpolation::table::Vector< Variant > >;

} // namespace resonances
