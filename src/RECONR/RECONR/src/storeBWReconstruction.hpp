template< typename BW >
static
void storeBWReconstruction( BW& bw, R2D2::ReconMap_t& reconstructed ){
  auto [ energies, XS ] = bw;

  // Remove units from x
  auto x = energies 
    | ranges::view::transform( []( auto&& energy ){ return energy.value; } );
  auto elastic = XS 
    | ranges::view::transform( []( auto&&  xs ){ return xs.elastic.value; } )
    | ranges::to_vector;
  auto fission = XS 
    | ranges::view::transform( []( auto&&  xs ){ return xs.fission.value; } )
    | ranges::to_vector;
  auto capture = XS 
    | ranges::view::transform( []( auto&&  xs ){ return xs.capture.value; } )
    | ranges::to_vector;

  reconstructed[ 2 ].push_back(
    interp::LinearLinear{ x | ranges::to_vector, std::move( elastic ) } );
  reconstructed[ 18 ].push_back(
    interp::LinearLinear{ x | ranges::to_vector, std::move( fission ) } );
  reconstructed[ 102 ].push_back(
    interp::LinearLinear{ x | ranges::to_vector, std::move( capture ) } );
}
