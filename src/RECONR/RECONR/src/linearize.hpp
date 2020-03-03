static
R2D2 linearize( ResonanceReconstructionDataDelivery& r2d2, 
                double relativeTolerance, double absoluteTolerance ){

  R2D2 linearized{};

  const auto infinity = std::numeric_limits<double>::infinity();

  auto criterion = [&]( auto&& trial, auto&& reference,
		       auto&& xLeft, auto&& xRight,
		       auto&&, auto&& ){
    if ( xRight == std::nextafter( xLeft, infinity ) ){ return true; }
    auto difference = std::abs( trial - reference );
    return ( difference < absoluteTolerance ) or
           ( (difference / reference) < relativeTolerance );
  };

  auto midpoint = []( auto&& x, auto&& ){
    return 0.5 * ( std::get<0>(x) + std::get<1>(x) );
  };
  
  for( const auto& [ MT, table ] : r2d2.crossSections() ){
    Log::info( "MT: {}", MT );
    auto eGrid = table.x() | ranges::to_vector;
    auto first = eGrid.begin();
    auto last = eGrid.end();
    std::vector< double > x;
    auto linearization = twig::linearize::callable< double >( x );
    linearization( first, last, table, criterion, midpoint );

    auto y = x | ranges::view::transform( table ) | ranges::to_vector;

    linearized.emplace( MT, 
      std::vector< interp::Variant >{ interp::Variant( 
            interp::LinearLinear( std::move( x ), std::move( y ) ) )
      }
    );
  }

  return linearized;
}