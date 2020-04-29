namespace resonanceParameters {

#include "RECONR/interp/src/resonanceParameters/SLBW.hpp"
#include "RECONR/interp/src/resonanceParameters/MLBW.hpp"
#include "RECONR/interp/src/resonanceParameters/RM.hpp"

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
