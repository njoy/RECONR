static
R2D2 linearize( ResonanceReconstructionDataDelivery& r2d2, 
                double relativeTolerance, double absoluteTolerance ){

  R2D2 linearized{};

  for( const auto& [ MT, table ] : r2d2.crossSections() ){
    std::vector< double > x;

    constexpr double infinity = std::numeric_limits<double>::infinity();

    auto criterion = [ & ]( auto&& trial, auto&& reference,
            auto&& xLeft, auto&& xRight,
            // auto&&, auto&&  ){
            auto&& yLeft, auto&& yRight ){

      if ( xRight == std::nextafter( xLeft, infinity ) ){ return true; }
      auto diff = std::abs( trial - reference );
      auto reldiff = (diff/reference);

      Log::info( R"(
criterion:
  trial: {:.16E}, refer: {:.16E}
  xleft: {:.16E}, xRight: {:.16E} 
  yleft: {:.16E}, yRight: {:.16E} 
  diffe: {:.16E}, reldif: {:.16E}
    )", trial, reference, xLeft, xRight, yLeft, yRight, diff, reldiff );
      return ( diff < absoluteTolerance ) or ( reldiff < relativeTolerance );
    };

    auto midpoint = []( auto&& x, auto&& ){
      return 0.5 * ( std::get<0>(x) + std::get<1>(x) );
    };
    
    auto eGrid = table.x() | ranges::to_vector;
    auto first = eGrid.begin();
    auto last = eGrid.end();
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
