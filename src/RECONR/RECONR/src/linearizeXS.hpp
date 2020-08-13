static
void linearizeXS( const Logger& logger, 
                  ResonanceReconstructionDataDelivery& r2d2, 
                  double relTol, double absTol ){

  linearizeXS( logger, r2d2.reactions(), relTol, absTol );
  linearizeXS( logger, r2d2.photonProductions(), relTol, absTol );
                                             
}

static
void linearizeXS( const Logger& logger,
                  R2D2::XSMap_t& reactions, 
                  double relTol, double absTol ){

  logger.first << "\nLinearizing cross sections." << std::endl;
  for( auto& [ id, reaction ] : reactions ){
    std::vector< interp::LinearLinear > linearized{};

    for( const auto& law : 
         reaction.crossSections< std::vector< interp::Variant > >() ){
      auto l = std::visit( 
          [&]( auto&& arg ){ 
            return njoy::RECONR::linearize( arg, relTol, absTol ); }, 
          law );
      linearized.emplace_back( l );
    }

    reaction.crossSections( std::move( linearized ) );
  }
}

static
void linearizeXS( const Logger& logger,
                  R2D2::PPMap_t& reactions, 
                  double relTol, double absTol ){

  logger.first << "\nLinearizing photon productions." << std::endl;
  for( auto & [ id, reaction ] : reactions ){
    std::vector< PPForms > linearized{};

    for( const auto& production : 
         reaction.productions< std::vector< interp::Variant > >() ){
      std::vector< interp::LinearLinear> lProd{};
      for( const auto& discrete : production ){
        auto l = std::visit( 
            [&]( auto&& arg ){ 
              return njoy::RECONR::linearize( arg, relTol, absTol ); }, 
            discrete );

        lProd.emplace_back( l );
      }
      linearized.emplace_back( interp::LinearTable( std::move( lProd ) ) );
    }
    reaction.productions( std::move( linearized ) );
  }
}
