PhotonYields( int ZA, double AWR, std::vector< PYForms >&& yields )
try:
  ZA_( ZA ),
  atomicWeightRatio_( AWR ),
  yields_( std::move( yields ) )
{ } catch( ... ){
  Log::info( "Trouble creating PhotonYields object." );
  throw;
}

PhotonYields() = delete;

