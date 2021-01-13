template< typename Range >
static
void reconstructCrossSections( const Logger& logger,
                               R2D2& r2d2,
                               const Range& energies ){

  logger.first 
    << "\nCalculating cross sections on unionized energy grid for IDs:\n";
  // Reconstruct everything
  // Perhaps this could be more efficient by only reconstructing those things
  // that are used, but I doubt that is the limiting factor here
  for( auto& [ ID, reaction ] : r2d2.reactions() ){
    auto mt = elementary::toEndfReactionNumber( ID );
    logger.first << fmt::format( "\t{:3} {:s}\n", mt, ID.symbol() );
    
    auto barns =  energies 
      | ranges::view::transform( 
          reaction.template crossSections< interp::LinearTable >() )
      | ranges::to_vector;

    reaction.crossSections( 
      XSForms{ std::make_pair( utility::copy( energies ), barns ) } );
  }
}
