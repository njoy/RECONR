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

  auto table = interp::LinearLinear{ 
    x | ranges::to_vector, std::move( elastic ) };
  // reconstructed.emplace( std::make_pair( 2,
  //   interp::LinearLinear{ x | ranges::to_vector, std::move( elastic ) } ) );
  // reconstructed.emplace( std::make_pair( 18,
  //   interp::LinearLinear{ x | ranges::to_vector, std::move( fission ) } ) );
  // reconstructed.emplace( std::make_pair( 102,
  //   interp::LinearLinear{ x | ranges::to_vector, std::move( capture ) } ) );
}

/*
      from
'njoy::interpolation::Table<
  njoy::interpolation::table::Type<
    njoy::interpolation::LinearLinear, 
    njoy::interpolation::table::Search< njoy::interpolation::Binary >,
    njoy::interpolation::table::Discontinuity< njoy::interpolation::TakeLeft >,
    std::__1::vector< double, std::__1::allocator<double> >, 
    std::__1::vector< double, std::__1::allocator<double> > 
  > 
>' 
      to 'const
'njoy::interpolation::table::Vector<
  njoy::interpolation::Table<
    njoy::interpolation::table::Type< njoy::interpolation::LinearLinear,
njoy::interpolation::table::Search<njoy::interpolation::Binary>,
njoy::interpolation::table::Discontinuity<njoy::interpolation::TakeLeft>,
std::__1::vector<double, std::__1::allocator<double> >, std::__1::vector<double,
std::__1::allocator<double> > > >, njoy::interpolation::Adjoining>' for 1st argument

 */
