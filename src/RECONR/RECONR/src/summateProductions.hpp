// For Photon production reactions
template< typename Range >
static
void summateProductions( const Logger& logger,
                         R2D2& data,
                         const Range& energies ){

  auto& productions = data.photonProductions();

  if( productions.size() <= 0 ){
    return;
  }

  logger.first << 
    "\nSumming photon production cross sections on unionized energy grid"
    " for IDs: \n";
  for( auto& [ ID, production ] : productions ){
    logger.first << fmt::format( "\t{:20s} ", ID.symbol() );

    std::vector< std::vector< double > > partials;
    for( const auto& partial : 
         production.template productions< interp::LinearTable >() ){
      auto barns =  energies 
        | ranges::view::transform( partial )
        | ranges::to_vector;

      partials.emplace_back( barns );
    }
    auto sum = sumPartials( partials );

    auto trunc = truncate( energies, sum );

    auto pairs = ranges::view::zip_with( 
      []( auto&& e, auto&& p ){ return PPForms{ std::make_pair( e, p ) }; },
      trunc.first, trunc.second )
      | ranges::to_vector;

    production.productions( std::move( pairs ) );
  }
  logger.first << std::endl;
}

