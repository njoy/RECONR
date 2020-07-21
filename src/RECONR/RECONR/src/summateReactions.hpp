// For incident neutrons
template< typename Range >
static
void summateReactions( std::ostream& output,
                       std::ostream& error,
                       R2D2& r2d2,
                       const Range& energies ){

  auto reactions = r2d2.reactions();
  const auto& projectile = r2d2.projectile();
  const auto& target = r2d2.target();

  // Sum redundant cross sections
  output << "Summing redundant cross sections for IDs:\n";
  for( const auto& [ MT, redundantMTs ] : 
       ranges::view::reverse( ENDFtk::redundantReactions ) ){

    auto mt2ID = [&]( auto&& mt ){ return
      elementary::fromEndfReactionNumber( projectile, target, mt ); };

    auto reactionID = mt2ID( MT );
    auto redundants = redundantMTs
      | ranges::view::filter( 
        [&]( auto&& mt ){ return reactions.count( mt2ID( mt ) ) > 0; } )
      | ranges::view::transform( 
        [&]( auto&& mt ){ return mt2ID( mt ); } );

    if( ranges::distance( redundants ) != 0 ){
      output << fmt::format( "{:s}, redundant IDs: ", MT );
      
      std::vector< std::vector< double > > partials;
      for( const auto& p : redundants ){
        output << fmt::format( "{:3s} ", p.symbol() );

        partials.push_back(
              reactions.at( p ).template crossSections< XSPair >().second );

      } // redundants

      auto rPair = std::make_pair( utility::copy( energies ), 
                                   sumPartials( partials ) );
      try {
        auto found = reactions.find( reactionID );
        if( found != reactions.end() ){
          reactions.at( reactionID ).crossSections( std::move( rPair ) );
        } else {
          // New reaction made entirely of summations
          auto id = redundants.front();
          auto rxn = reactions.at( id );
          reactions.emplace( reactionID, 
            Reaction{ rxn.ZA(), rxn.AWR(), 0.0, 0.0, 0, std::move( rPair ) }
          );
        }
      } catch( std::exception& ){
        Log::error( "'total' cross section not orginally given for ID: {}", 
                    reactionID.symbol() );
        throw;
      }
      output << std::endl;
    }
  } // Redundant cross sections

}
