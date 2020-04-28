// These are taken from interpolation/src/test/Example8.cpp
using Histogram              = Table< interpolation::Histogram >;
using LinearLinear           = Table< interpolation::LinearLinear >;
using LinearLogarithmic      = Table< interpolation::LinearLogarithmic >;
using LogarithmicLinear      = Table< interpolation::LogarithmicLinear >;
using LogarithmicLogarithmic = Table< interpolation::LogarithmicLogarithmic >;

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


