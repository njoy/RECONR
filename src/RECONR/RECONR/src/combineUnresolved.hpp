
template< typename Range >
static
void combineUnresolved( std::ostream& output,
                        std::ostream& error,
                        R2D2& r2d2,
                        const Range& energies ){
  auto& reactions = r2d2.reactions();
  const auto& proj = r2d2.projectile();
  const auto& target = r2d2.target();
  auto& unresolved = r2d2.unresolved();

  if( unresolved.empty() ){ return; }

  output << "Combining unresolved resonances with background." << std::endl;

  const auto& [ uID, uRxn ] = *( unresolved.begin() );
  const auto& uEnergies = uRxn.crossSections< interp::LinearLinear >().x();

  std::vector< std::vector< double > > partials;
  std::vector< double > sumUXS( ranges::distance( uEnergies ), 0.0 );

  const ReactionID fissionTotal{ 
    proj, target, elementary::ReactionType{ "fission[total]" } };
  const ReactionID fission{ 
    proj, target, elementary::ReactionType{ "fission" } };

  output << "\nAdding reconstructed unresolved cross sections to background\n";
  for( const auto& ID : ranges::view::keys( unresolved ) ){

    auto addUnresolved = [&]( const ReactionID& rxnID ){
      output << fmt::format( "\t{:s}\n", rxnID.symbol() );
      auto& unres = unresolved.at( ID );
      auto& reaction = reactions.at( rxnID );
      const auto& rxn = reaction.crossSections< interp::LinearTable >();

      auto eL = r2d2.resolvedRange().second;
      auto eH = r2d2.unresolvedRange().second;

      const auto& table = unres.crossSections< interp::LinearLinear >();

      std::vector< double > unresXS;
      if( unres.LSSF() ){
        unresXS = uEnergies
          | ranges::view::transform(
              [&]( auto&& e ){ return ( ( eL <= e ) && ( e <= eH ) ) 
                ? table( e ) : 0.0; } 
            )
          | ranges::to_vector;

        partials.push_back(
          uEnergies | ranges::view::transform( rxn ) | ranges::to_vector );

      } else {
        unresXS = uEnergies
          | ranges::view::transform(
              [&]( auto&& e ){ return ( ( eL <= e ) && ( e <= eH ) )
                ? table( e ) + rxn( e ) : 0.0; } 
            )
          | ranges::to_vector;
        
        auto y = energies 
          | ranges::view::transform(
            [&]( auto&& e ){ return table( e ) + rxn( e ); } )
          | ranges::to_vector;

        std::vector< interp::Variant > XS{
          interp::LinearLinear{ energies | ranges::to_vector, std::move( y ) }
        };
        reaction.crossSections( std::move( XS ) );
      }

      sumUXS = ranges::view::zip_with( std::plus< double >(), unresXS, sumUXS );

      unres.crossSections( 
        std::make_pair( utility::copy( uEnergies ), unresXS ) );
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

  ReactionID tID{ proj, target, elementary::ReactionType{ "total" } };
  const auto& tRxn = 
    reactions.at( tID ).template crossSections< interp::LinearTable >();

  auto tXS = uEnergies
    | ranges::view::transform( tRxn )
    | ranges::to_vector;

  if( uRxn.LSSF() ){

    auto parts = sumPartials( partials );
    auto comp = ranges::view::zip_with( std::minus< double >(), tXS, parts );
    auto total = ranges::view::zip_with( std::plus< double >(), comp, sumUXS )
      | ranges::to_vector;
    
    URxn uTotal{ uRxn.ZA(), uRxn.AWR(), uRxn.LSSF(), 
                std::make_pair( utility::copy( uEnergies ), 
                                std::move( total ) ) 
    };
    unresolved.emplace( tID, std::move( uTotal ) );
  }
  output << std::endl;
}