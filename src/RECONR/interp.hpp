namespace interp{

// These are taken from interpolation/src/test/Example8.cpp
template< typename I >
using LAW = decltype( 
  interpolation::table::make< I >( 
    std::vector< double >(), std::vector< double >() ) );

/*
using LAW = interpolation::Table<
  interpolation::table::Type< I,
    interpolation::table::search::Binary,
    interpolation::table::discontinuity::TakeLeft,
    std::vector< double >, std::vector< double > 
  >
>;
*/

using Histogram              = LAW< interpolation::Histogram >;
using LinearLinear           = LAW< interpolation::LinearLinear >;
using LinearLogarithmic      = LAW< interpolation::LinearLogarithmic >;
using LogarithmicLinear      = LAW< interpolation::LogarithmicLinear >;
using LogarithmicLogarithmic = LAW< interpolation::LogarithmicLogarithmic >;

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
} // namespace interp
