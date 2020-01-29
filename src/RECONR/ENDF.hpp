namespace ENDF{

// These are taken from interpolation/src/test/Example8.cpp
template< typename I >
using LAW = decltype( 
  interpolation::table::make< I >( 
    std::vector< double >(), std::vector< double >() ) );

using Law1 = LAW< interpolation::Histogram >;
using Law2 = LAW< interpolation::LinearLinear >;
using Law3 = LAW< interpolation::LinearLogarithmic >;
using Law4 = LAW< interpolation::LogarithmicLinear >;
using Law5 = LAW< interpolation::LogarithmicLogarithmic >;

using Variant = interpolation::Table< 
    interpolation::table::Variant< Law1, Law2, Law3, Law4, Law5 > >;
using Tab1 = interpolation::Table< interpolation::table::Vector< Variant > >;

auto partition = []( auto&& range, int drop, int take ){
  return range
      | ranges::view::drop_exactly(drop)
      | ranges::view::take_exactly(take);
};
} // namespace laws
