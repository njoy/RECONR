static
void linearizeXS( std::ostream& output,
                  ResonanceReconstructionDataDelivery& r2d2, 
                  double relTol, double absTol ){

  output << "\nLinearizing cross sections." << std::endl;

  r2d2.linearReactions( linearizeXS( output, r2d2.reactions(),
                                     relTol, absTol ) );
  r2d2.linearPhotonProductions( linearizeXS( output, r2d2.photonProductions(),
                                             relTol, absTol ) );
                                             

}

static
R2D2::LinMap_t linearizeXS( std::ostream& output,
                  const R2D2::XSMap_t& reactions, 
                  double relTol, double absTol ){

  R2D2::LinMap_t linearMap{};

  for( const auto& [ MT, reaction ] : reactions ){
    std::vector< interp::LinearLinear > linearized{};

    for( const auto& law : reaction.crossSections() ){
      auto l = std::visit( 
          [&]( auto&& arg ){ 
            return njoy::RECONR::linearize( arg, relTol, absTol ); }, 
          law );
      linearized.emplace_back( l );
    }

    Reaction< interp::LinearTable > lReaction( reaction,
      interp::LinearTable( std::move( linearized ) ) );
    linearMap.emplace( MT, std::move( lReaction ) );
  }

  return linearMap;
}

static
R2D2::PPLinMap_t linearizeXS( std::ostream& output,
                  const R2D2::PPMap_t& reactions, 
                  double relTol, double absTol ){

  R2D2::PPLinMap_t prodMap{};
  for( const auto & [ MT, reaction ] : reactions ){
    std::vector< interp::LinearTable > linearized{};

    for( const auto& production : reaction.productions() ){
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
    PPReaction< interp::LinearTable > lppReaction( 
      reaction, std::move( linearized ) );

    prodMap.emplace( MT, std::move( lppReaction ) );

  }

  return prodMap;
}
