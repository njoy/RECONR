/*
 * Combine reconstructed resonances with background cross sections
 */
template< typename Range >
static
void combineReconstructed( std::ostream& output,
                           std::ostream& error,
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
  output << "\nAdding reconstructed cross sections to background for IDs:\n";
  for( const auto& ID : ranges::view::keys( reconstructed ) ){
    std::vector< std::vector< double >  > partials;

    auto addReconstructed = [&]( const ReactionID& rxnID ){
      output << fmt::format( "\t{:s}\n", rxnID.symbol() );
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
  output << std::endl;

  output << "\nAdding reconstructed unresolved cross sections to background\n";
  auto& unresolved = r2d2.unresolved();
  for( const auto& ID : ranges::view::keys( unresolved ) ){

    auto addUnresolved = [&]( const ReactionID& rxnID ){
      output << fmt::format( "\t{:s}\n", rxnID.symbol() );
      auto& unres = unresolved.at( ID );
      auto& reaction = reactions.at( rxnID );

      auto eL = r2d2.resolvedRange().second;
      auto eH = r2d2.unresolvedRange().second;

      auto table = unres.crossSections< interp::LinearLinear >();
      auto unresXS = energies
        | ranges::view::transform(
            [&]( auto&& e ){
              return ( ( eL < e ) && ( e < eH ) ) ? table( e ) : 0.0; } )
        | ranges::to_vector;

      auto xs = ranges::view::zip_with(
        std::plus< double >(),
        reaction.template crossSections< XSPair >().second, unresXS )
        | ranges::to_vector;

      unres.crossSections( 
        std::make_pair( utility::copy( energies ), xs ) );
      if( not unres.LSSF() ){
        reaction.crossSections( 
          std::make_pair( utility::copy( energies ), xs ) );
      }
    };

    if( ID == fission ){
      if( reactions.find( fission ) != reactions.end() ){
        addUnresolved( fission );
      } else if (reactions.find( fissionTotal ) != reactions.end() ) {
        addUnresolved( fissionTotal );
      }
    } else{
      if( reactions.find( ID ) != reactions.end() ){
        addUnresolved( ID );
      }
    }
    
  }

  output << std::endl;
}
