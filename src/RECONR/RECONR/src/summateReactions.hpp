template< typename Range >
static
std::vector< double > sumPartials( const Range& partials ){

  auto sumTuple = []( auto&& tuple ){ 
    return std::apply(
      [](auto... v) { return (v + ...); },
      tuple 
    );
  };

  return ranges::accumulate( partials,
    std::vector< double >( partials[0].size() ),
    [&]( const auto& acc, const auto& next ){
      return ranges::view::zip( acc, next )
        | ranges::view::transform( sumTuple )
        | ranges::to_vector;
    }
  );
}

template< typename Range >
static
auto summateReactions( ResonanceReconstructionDataDelivery& r2d2,
                       Range& energies ){
  std::map< int, std::vector< double > > reactions;
  
  const auto linear = r2d2.linearCrossSections();
  const auto reconstructed = r2d2.reconstructedResonances();

  auto keys = ranges::view::keys( linear );
  auto recKeys = ranges::view::keys( reconstructed );

  auto sumTuple = []( auto&& tuple ){ 
    return std::apply(
      [](auto... v) { return (v + ...); },
      tuple 
    );
  };

  // Reconstruct everything
  for( const auto& [ MT, XS ] : linear ){
    
    reactions[ MT ] =  energies 
      | ranges::view::transform( XS )
      | ranges::to_vector;
  }

  // Add reconstructed resonances
  for( const auto& [ MT, recon ] : reconstructed ){
    std::vector< std::vector< double >  > partials{ reactions.at( MT ) };

    // Get cross sections from reconstructed reactions
    for( const auto& XS : recon ){
      auto partial = energies | ranges::view::transform( XS );
      partials |= ranges::action::push_back( partial );
    }
    reactions[ MT ] = sumPartials( partials );
  } // Reconstructed resonances

  // Sum redundant cross sections
  for( const auto& [ MT, redundantMTs ] : 
       ranges::view::reverse( ENDFtk::redundantReactions ) ){

    if( MT == 27 ) continue;

    auto redundants = redundantMTs
      | ranges::view::filter( 
        [&]( auto&& mt ){ return reactions.count( mt ); } );

    if( ranges::distance( redundants ) != 0 ){
      
      std::vector< std::vector< double > > partials;
      for( const auto& p : redundants ){
        auto found = reactions.find( p );

        if( found != reactions.end() ){
          partials |= ranges::action::push_back( found->second );
        } else{
          auto foundReaction = linear.find( p );
          if( foundReaction != linear.end() ){
            auto part = foundReaction->second;
            auto party = part.x() 
              | ranges::view::transform( part ) 
              | ranges::to_vector;
            partials |= ranges::action::push_back( party );
          }
        }

        reactions[ MT ] = sumPartials( partials );
      }
    }
  } // Redundant cross sections
  return reactions;
}
