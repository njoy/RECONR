UnresolvedReaction( int ZA, double AWR, bool lssf,
                    UForms&& xs )
try:
  ZA_( ZA ),
  atomicWeightRatio_( AWR ),
  lssf_( lssf ),
  crossSection_( std::move( xs ) )
{ } catch( ... ){
  Log::info( "Trouble creating UnresolvedReaction object." );
  throw;
}

UnresolvedReaction() = delete;
