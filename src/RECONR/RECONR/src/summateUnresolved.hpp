// This method calculates the total unresolved cross section
// It takes care of the different monstrosities that are in the ENDF format
static
void summateUnresolved( const Logger& logger, R2D2& r2d2 ){

  const auto& proj = r2d2.projectile();
  const auto& target = r2d2.target();

  auto& unresolved = r2d2.unresolved();

  if( unresolved.empty() ){ return; }

  const auto& [ uID, uRxn ] = *( unresolved.begin() );
  auto energies = uRxn.template crossSections< XSPair >().first;

  if( uRxn.LSSF() ){ return; }

  logger.first << "Summating total unresolved resonance." << std::endl;

  ReactionID tID{ proj, target, elementary::ReactionType{ "total" } };
  const auto& [ tE, tXS ] = 
    r2d2.reactions().at( tID ).template crossSections< XSPair >();

  auto begin = tE.begin();
  auto end = tE.end();

  std::vector< double > xs;
  xs.reserve( energies.size() );
  for( const auto& e : energies ){

    auto f = std::find( begin, end, e );
    if( f != end ){
      auto index = std::distance( begin, f );
      xs.push_back( tXS[ index ] );
    } else {
      xs.push_back( 0.0 );
    }
  }

  URxn uTotal{ uRxn.ZA(), uRxn.AWR(), uRxn.LSSF(), 
               std::make_pair( std::move( energies ), std::move( xs ) ) 
  };
  unresolved.emplace( tID, std::move( uTotal ) );
}
