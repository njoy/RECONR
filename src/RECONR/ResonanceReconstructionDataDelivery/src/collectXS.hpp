XSmap_t collectXS(){
  XSmap_t xs{};

  auto MF3 = this->material.fileNumber( 3 ).parse< 3 >();
  for( auto& section : MF3.sections() ){
    std::vector< ENDF::Variant > cs;

    auto barns = section.crossSections() | ranges::to_vector;
    auto energies = section.energies() | ranges::to_vector;

    auto makeLAWTable = [&]( int drop, int take, auto& LAW ){
      auto partEnergies = ENDF::partition( energies, drop,  take );
      auto partBarns = ENDF::partition( barns, drop,  take );

      switch( LAW ){
        case 1:
          return ENDF::Variant( ENDF::Law1( std::move( partEnergies ), 
                                    std::move( partBarns ) ) );
          break;
        case 2:
          return ENDF::Variant( ENDF::Law2( std::move( partEnergies ), 
                                    std::move( partBarns ) ) );
          break;
        case 3:
          return ENDF::Variant( ENDF::Law3( std::move( partEnergies ), 
                                    std::move( partBarns ) ) );
          break;
        case 4:
          return ENDF::Variant( ENDF::Law4( std::move( partEnergies ), 
                                    std::move( partBarns ) ) );
          break;
        case 5:
          return ENDF::Variant( ENDF::Law5( std::move( partEnergies ), 
                                    std::move( partBarns ) ) );
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
    cs.push_back( makeLAWTable( drop, take, interpolants[ 0 ] ) );
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
      cs.push_back( makeLAWTable( drop, take, std::get< 0 >( params ) ) );
    }
    xs.insert( std::make_pair( section.MT(), ENDF::Tab1( std::move( cs ) ) ) );
  }

  return xs;
}
