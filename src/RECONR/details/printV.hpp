namespace details{

template< typename G >
void printV( std::string name, const G& grid ){
  njoy::Log::info( "{}, {}", name, ranges::distance( grid ) );
  njoy::Log::info( "{}", grid | ranges::view::all );
}

} // namespace details
