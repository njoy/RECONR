static
void linearize( ResonanceReconstructionDataDelivery& r2d2, 
                double relTol, double absTol ){

  R2D2::LinMap_t linearMap{};

  for( const auto& [ MT, table ] : r2d2.crossSections() ){
    Log::info( "MT: {}--------------------------", MT );
    std::vector< interp::LinearLinear > linearized{};

    for( const auto& law : table ){
      auto l = std::visit( 
          [&]( auto&& arg ){ 
            Log::info( "x: {}", arg.x() | ranges::view::all );
            Log::info( "y: {}", arg.y() | ranges::view::all );
            return njoy::RECONR::linearize( arg, relTol, absTol ); }, 
          law );
      // Log::info( "l.x: {}", l.x() | ranges::view::all );
      linearized.emplace_back( l );
    }

    linearMap.emplace( MT, std::move( linearized ) );
  }
}

