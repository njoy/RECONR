namespace interp{

template< typename I >
using Table = decltype( 
  interpolation::table::make< I >( 
    std::vector< double >(), std::vector< double >() ) );

#include "RECONR/interp/src/ENDF.hpp"
#include "RECONR/interp/src/resonanceParameters.hpp"
} // namespace interp
