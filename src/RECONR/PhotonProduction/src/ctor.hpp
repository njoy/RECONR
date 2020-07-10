PhotonProduction( int ZA, double AWR, std::vector< PPForms >&& prods )
try:
  ZA_( ZA ),
  atomicWeightRatio_( AWR ),
  productions_( std::move( prods ) )
{ } catch( ... ){
  Log::info( "Trouble creating PhotonProduction object." );
  throw;
}

PhotonProduction() = delete;
