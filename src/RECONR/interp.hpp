namespace interp{

struct Zero {
  static constexpr double value = 0.0;
};

// These are taken from interpolation examples 6 and 8
template< typename I >
using LAW = decltype( 
  interpolation::table::make< I,
  interpolation::table::left::interval::IsCompiletimeConstant<Zero>,
  interpolation::table::right::interval::IsCompiletimeConstant<Zero>
  >( 
    std::vector< double >(), std::vector< double >() ) );

using Histogram              = LAW< interpolation::Histogram >;
using LinearLinear           = LAW< interpolation::LinearLinear >;
using LinearLogarithmic      = LAW< interpolation::LinearLogarithmic >;
using LogarithmicLinear      = LAW< interpolation::LogarithmicLinear >;
using LogarithmicLogarithmic = LAW< interpolation::LogarithmicLogarithmic >;

// For convenience
using Histog = Histogram;
using LinLin = LinearLinear;
using LinLog = LinearLogarithmic;
using LogLin = LogarithmicLinear;
using LogLog = LogarithmicLogarithmic;

using Variant = std::variant< 
      Histogram, 
      LinearLinear, 
      LinearLogarithmic, 
      LogarithmicLinear, 
      LogarithmicLogarithmic >;

using LinearTable = interpolation::Table< 
  interpolation::table::Vector< LinearLinear > >;

auto partition = []( auto&& range, int drop, int take ){
  return range
      | ranges::view::drop_exactly(drop)
      | ranges::view::take_exactly(take);
};

#include "RECONR/interp/src/makeInterpolationTable.hpp"
#include "RECONR/interp/src/MF3toInterpolation.hpp"
#include "RECONR/interp/src/MF13toInterpolation.hpp"
} // namespace interp
