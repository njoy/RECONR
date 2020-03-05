XSmap_t collectXS( const Material_t material){
  return std::visit( 
    [&](auto&& arg ){ return this->collectXS( arg ); },
    material );
}

XSmap_t collectXS( const ENDFMaterial_t& material ){
  XSmap_t xs{};

  auto MF3 = material.fileNumber( 3 ).parse< 3 >();
  for( auto& section : MF3.sections() ){
    std::vector< interp::Variant > cs;

    // We have to convert to a vector to "own" the data
    auto energies = section.energies() | ranges::to_vector;
    auto barns = section.crossSections() | ranges::to_vector;

    auto makeInterpTable = [&]( int drop, int take, auto& LAW ){
      auto pE = interp::partition( energies, drop,  take );
      auto pB = interp::partition( barns, drop,  take );

      Log::info( "LAW={}", LAW );
      switch( LAW ){
        case 1:
          cs.emplace_back( interp::Variant( 
              interp::Histogram( std::move( pE ), std::move( pB ) ) ) );
          break;
        case 2:
          cs.emplace_back( interp::Variant( 
            interp::LinearLinear( std::move( pE ), std::move( pB ) ) ) );
          break;
        case 3:
          cs.emplace_back( interp::Variant( 
            interp::LinearLogarithmic( std::move( pE ), std::move( pB ) ) ) );
          break;
        case 4:
          cs.emplace_back( interp::Variant( 
            interp::LogarithmicLinear( std::move( pE ), std::move( pB ) ) ) );
          break;
        case 5:
          cs.emplace_back( interp::Variant( 
            interp::LogarithmicLogarithmic( 
                std::move( pE ), std::move( pB ) ) ) );
          break;
        default:
          Log::error( "Invalid interpolation parameter." );
          throw std::exception();
      }
    };

    auto interpolants = section.interpolants();
    auto boundaries = section.boundaries();
    int left = 0;
    int right = boundaries[ 0 ];
    int drop = 0;
    int take = right;
    makeInterpTable( drop, take, interpolants[ 0 ] );
    for( const auto& params : 
        ranges::view::zip( interpolants, boundaries ) 
          | ranges::view::drop_exactly( 1 ) ){

      left = right;
      right = std::get< 1 >( params );
      take = right - left;
      drop = left;

      // interpolation needs to have double points
      if( energies[ left ] != energies[ left + 1] ){
        drop -= 1;
        take +=1;
      }
      makeInterpTable( drop, take, std::get< 0 >( params ) );
    }
    xs.insert( 
      std::make_pair( section.MT(), interp::Table( std::move( cs ) ) ) );
  }

  return xs;
}
