XSmap_t collectXS(){
  XSmap_t xs{};

  auto MF3 = this->material.fileNumber( 3 ).parse< 3 >();
  for( auto& section : MF3.sections() ){
    std::vector< ENDF::Variant > cs;
    if(section.MT() != 17 ) continue;
    Log::info( "MT: {}>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>", section.MT() );

    auto energies = section.energies() | ranges::to_vector;
    auto barns = section.crossSections() | ranges::to_vector;

    // Log::info( "\tbarns: {}", barns | ranges::view::all );
    // Log::info( "\tenergies: {}", energies | ranges::view::all );

    auto makeLAWTable = [&]( int drop, int take, auto& LAW ){
      auto pE = ENDF::partition( energies, drop,  take );
      auto pB = ENDF::partition( barns, drop,  take );

      switch( LAW ){
        case 1:
          return ENDF::Variant( ENDF::Law1( 
            std::move( pE ), std::move( pB ) ) );
          break;
        case 2:
          return ENDF::Variant( ENDF::Law2(
            std::move( pE ), std::move( pB ) ) );
          break;
        case 3:
          return ENDF::Variant( ENDF::Law3(
            std::move( pE ), std::move( pB ) ) );
          break;
        case 4:
          return ENDF::Variant( ENDF::Law4( 
            std::move( pE ), std::move( pB ) ) );
          break;
        case 5:
          return ENDF::Variant( ENDF::Law5( 
            std::move( pE ), std::move( pB ) ) );
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
    cs.emplace_back( makeLAWTable( drop, take, interpolants[ 0 ] ) );
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
      cs.emplace_back( makeLAWTable( drop, take, std::get< 0 >( params ) ) );
    }
    auto p = std::make_pair( section.MT(), ENDF::Tab1( std::move( cs ) ) );
    // auto table = std::get< 1 >( p );
    // auto x = table.x() | ranges::to_vector;
    // auto y = table.y() | ranges::to_vector;
    // Log::info( "x: {}", x | ranges::view::all );
    // Log::info( "y: {}", y | ranges::view::all );
    xs.insert( std::move( p ) );
    Log::info( "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<MT: {}", section.MT() );
  }

  return xs;
}
