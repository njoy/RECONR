static
void linearizeXS( std::ostream& output,
                  ResonanceReconstructionDataDelivery& r2d2, 
                  double relTol, double absTol ){

  output << "\nLinearizing cross sections." 
         << std::endl;

  R2D2::LinMap_t linearMap{};

  for( const auto& [ MT, reaction ] : r2d2.reactions() ){
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
  r2d2.linearReactions( std::move( linearMap ) );

  // R2D2::PPLinMap_t prodMap{};
  // for( const auto & [ MT, productions ] : r2d2.photonProductions() ){
  // }
}
