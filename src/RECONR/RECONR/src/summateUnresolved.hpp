// This method calculates the total unresolved cross section
// It takes care of the different monstrosities that are in the ENDF format
static
void summateUnresolved( std::ostream& output,
                        std::ostream& error,
                        R2D2& r2d2 ){

  const auto& proj = r2d2.projectile();
  const auto& target = r2d2.target();

  auto& unresolved = r2d2.unresolved();

  if( unresolved.empty() ){ return; }

  const auto& [ uID, uRxn ] = *( unresolved.begin() );
  auto energies = uRxn.template crossSections< XSPair >().first;

  ReactionID tID{ proj, target, elementary::ReactionType{ "total" } };
  const auto& [ tE, tXS ] = 
    r2d2.reactions().at( tID ).template crossSections< XSPair >();

  auto& tU = unresolved.at( tID );
  if( not tU.LSSF() ){

    auto begin = tE.begin();
    auto end = tE.end();

    std::vector< double > xs( energies.size(), 0.0 );
    for( const auto& e : energies ){

      auto f = std::find( begin, end, e );
      if( f != end ){
        auto index = std::distance( begin, f );
        xs.push_back( tXS[ index ] );
      }
    }

    URxn uTotal{ uRxn.ZA(), uRxn.AWR(), uRxn.LSSF(), 
                 std::make_pair( std::move( energies ), std::move( xs ) ) 
    };
    unresolved.emplace( tID, std::move( uTotal ) );
  }
}
