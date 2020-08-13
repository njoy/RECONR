/*
 * Combine reconstructed resonances with background cross sections
 */
template< typename Range >
static
void combineReconstructed( const Logger& logger,
                           R2D2& r2d2,
                           const Range& energies ){

  auto& reactions = r2d2.reactions();
  const auto& reconstructed = r2d2.reconstructedResonances();
  const auto& proj = r2d2.projectile();
  const auto& target = r2d2.target();

  const auto fissionTotal = 
    elementary::fromEndfReactionNumber( proj, target, 18 );
  const auto fission = 
    elementary::fromEndfReactionNumber( proj, target, 19 );

  double eL{ 0.0 };
  double eH{ 1E10 };
  logger.first 
    << "\nAdding reconstructed cross sections to background for IDs:\n";
  for( const auto& ID : ranges::view::keys( reconstructed ) ){
    auto mt = elementary::toEndfReactionNumber( ID );

    std::vector< std::vector< double >  > partials;

    auto addReconstructed = [&]( const ReactionID& rxnID ){
      logger.first << fmt::format( "\t{:3} {:s}\n", mt, ID.symbol() );
      const auto& recon = reconstructed.at( ID );
      auto& reaction = reactions.at( rxnID );
      auto& part = reaction.template crossSections< XSPair >().second;
      partials.push_back( part );

      for( auto& XS : recon ){
        auto partial = energies | ranges::view::transform( XS );
        partials.push_back( partial );
      }
      auto sum = sumPartials( partials );
      reaction.crossSections( 
        std::make_pair( utility::copy( energies ), std::move( sum ) ) );
    };

    if( ID == fission ){
      if( reactions.find( fission ) != reactions.end() ){
        addReconstructed( fission );
      } else if (reactions.find( fissionTotal ) != reactions.end() ) {
        addReconstructed( fissionTotal );
      }
    } else{
      if( reactions.find( ID ) != reactions.end() ){
        addReconstructed( ID );
      }
    }
  } // Reconstructed resonances
  logger.first << std::endl;

}
