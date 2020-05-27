Reaction( int ZA, double AWR, double QM, double QI, int LR,
         XS&& xs )
try:
  ZA_( ZA ),
  atomicWeightRatio_( AWR ),
  massDifferenceQ_( QM ),
  reactionQ_( QI ),
  complexBreakUp_( LR ),
  crossSection_( std::forward< XS >( xs ) )
{ } catch( ... ){
  Log::info( "Trouble creating Reaction object." );
  throw;
}

template< typename R >
Reaction( const R& reaction, XS&& xs )
try:
  ZA_( reaction.ZA() ),
  atomicWeightRatio_( reaction.AWR() ),
  massDifferenceQ_( reaction.QM() ),
  reactionQ_( reaction.QI() ),
  complexBreakUp_( reaction.LR() ),
  crossSection_( std::forward< XS >( xs ) )
{ } catch( ... ){
  Log::info( "Trouble creating Reaction object from another Reaction." );
  throw;
}

Reaction() = delete;
