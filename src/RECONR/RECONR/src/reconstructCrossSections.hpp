template< typename Range >
static
void reconstructCrossSections( std::ostream& output,
                              std::ostream& error,
                              R2D2& r2d2,
                              const Range& energies ){

  output << "\nCalculating cross sections on unionized energy grid for IDs:\n";
  // Reconstruct everything
  // Perhaps this could be more efficient by only reconstructing those things
  // that are used, but I doubt that is the limiting factor here
  for( auto& [ ID, reaction ] : r2d2.reactions() ){
    output << fmt::format( "\t{:s}\n", ID.symbol() );
    
    auto barns =  energies 
      | ranges::view::transform( 
          reaction.crossSections< interp::LinearTable >() )
      | ranges::to_vector;

    reaction.crossSections( 
      XSForms{ std::make_pair( utility::copy( energies ), barns ) } );
  }
}
