namespace ENDF{

// These are taken from interpolation/src/test/Example8.cpp
std::vector< double > p_;
using Partition = decltype( 
    p_ | ranges::view::drop_exactly(0) | ranges::view::take_exactly(0) );

using Law1 = decltype( 
  interpolation::table::make< interpolation::Histogram >( 
    std::declval<Partition>(), std::declval<Partition>() ) );
using Law2 = decltype( 
  interpolation::table::make< interpolation::LinearLinear >( 
    std::declval<Partition>(), std::declval<Partition>() ) );
using Law3 = decltype( 
  interpolation::table::make< interpolation::LinearLogarithmic >( 
    std::declval<Partition>(), std::declval<Partition>() ) );
using Law4 = decltype( 
  interpolation::table::make< interpolation::LogarithmicLinear >( 
    std::declval<Partition>(), std::declval<Partition>() ) );
using Law5 = decltype( 
  interpolation::table::make< interpolation::LogarithmicLogarithmic >( 
    std::declval<Partition>(), std::declval<Partition>() ) );

using Variant = interpolation::Table< 
    interpolation::table::Variant< Law1, Law2, Law3, Law4, Law5 > >;
using Tab1 = interpolation::Table< interpolation::table::Vector< Variant > >;

auto partition = []( auto&& range, int drop, int take ){
  return range
      | ranges::view::drop_exactly(drop)
      | ranges::view::take_exactly(take);
};
} // namespace laws
