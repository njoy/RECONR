static
void linearizeXS( ResonanceReconstructionDataDelivery& r2d2, 
                double relTol, double absTol ){

  R2D2::LinMap_t linearMap{};

  for( const auto& [ MT, table ] : r2d2.crossSections() ){
    std::vector< interp::LinearLinear > linearized{};

    for( const auto& law : table ){
      auto l = std::visit( 
          [&]( auto&& arg ){ 
            return njoy::RECONR::linearize( arg, relTol, absTol ); }, 
          law );
      linearized.emplace_back( l );
    }

    linearMap.emplace( MT, interp::LinearTable( std::move( linearized ) ) );
  }
  r2d2.linearCrossSections( std::move( linearMap ) );
}
