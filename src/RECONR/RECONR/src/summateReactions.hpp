template< typename Range >
static
auto summateReactions( ResonanceReconstructionDataDelivery& r2d2,
                       Range& energies ){
  std::map< int, std::vector< double > > reactions;
  
  const auto linear = r2d2.linearCrossSections();
  const auto reconstructed = r2d2.reconstructedResonances();

  auto keys = ranges::view::keys( linear );
  auto recKeys = ranges::view::keys( reconstructed );

  for( const auto& [ MT, XS ] : ranges::view::reverse( linear ) ){
    Log::info( "MT: {}", MT );

    if( ENDFtk::isRedundant( MT ) ){
      auto redundancies = ENDFtk::redundantReactions.at( MT );

      Log::info( "redundancies: {}", redundancies | ranges::view::all );
      auto redundants = redundancies 
        | ranges::view::filter(
          [&]( auto&& mt ){ return linear.count( mt ); }
          );
        
      if( ranges::distance( redundants ) != 0 ){

        std::vector< std::vector< double > > partials;
        for( const auto& p : redundants ){
          auto part = linear.at( p );
          auto party = part.x() 
            | ranges::view::transform( part ) 
            | ranges::to_vector;

          Log::info( "\t\tparty: {}", party | ranges::view::all  );
          partials |= ranges::action::push_back( party );
        }

        auto sumTuple = []( auto&& tuple ){ 
          Log::info( "\t\t\ttuple:" );
          return std::apply(
            [](auto... v) { return (v + ...); },
            tuple 
          );
        };
        auto y = ranges::view::zip( partials )
          | ranges::view::transform( sumTuple )
          | ranges::to_vector;
        // Log::info( "y: {}", y | ranges::view::all );
        // reactions[ MT ] = std::move( y );
      }
    } else { // Not a redundant reaction
      auto y = energies 
        | ranges::view::transform( XS )
        | ranges::to_vector;
      // Log::info( "y: {}", y | ranges::view::all );

      reactions[ MT ] = std::move( y );
    }

  }

  return reactions;
}
