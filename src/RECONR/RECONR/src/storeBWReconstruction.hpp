template< typename BW >
static
void storeBWReconstruction( BW& bw, R2D2& r2d2 ){
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

  auto& reconstructed = r2d2.reconstructedResonances();
  auto elasticR = elementary::fromEndfReactionNumber( 
      r2d2.projectile(), r2d2.target(), 2 );
  auto fissionR = elementary::fromEndfReactionNumber( 
      r2d2.projectile(), r2d2.target(), 19 );
  auto captureR = elementary::fromEndfReactionNumber( 
      r2d2.projectile(), r2d2.target(), 102 );

  reconstructed[ elasticR ].push_back(
    interp::LinearLinear{ x | ranges::to_vector, std::move( elastic ) } );
  reconstructed[ fissionR ].push_back(
    interp::LinearLinear{ x | ranges::to_vector, std::move( fission ) } );
  reconstructed[ captureR ].push_back(
    interp::LinearLinear{ x | ranges::to_vector, std::move( capture ) } );
}
