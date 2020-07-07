Reaction( int ZA, double AWR, double QM, double QI, int LR,
         Forms&& xs )
try:
  ZA_( ZA ),
  atomicWeightRatio_( AWR ),
  massDifferenceQ_( QM ),
  reactionQ_( QI ),
  complexBreakUp_( LR ),
  crossSection_( std::move( xs ) )
{ } catch( ... ){
  Log::info( "Trouble creating Reaction object." );
  throw;
}

Reaction() = delete;
