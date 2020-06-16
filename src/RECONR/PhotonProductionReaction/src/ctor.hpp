PhotonProductionReaction( int ZA, double AWR, 
                          std::vector< XS >&& xs )
try:
  ZA_( ZA ),
  atomicWeightRatio_( AWR ),
  productions_( std::forward< std::vector< XS > >( xs ) )
{ } catch( ... ){
  Log::info( "Trouble creating PhotonProductionReaction object." );
  throw;
}

template< typename R >
PhotonProductionReaction( const R& reaction,
                          std::vector< XS >&& xs )
try:
  ZA_( reaction.ZA() ),
  atomicWeightRatio_( reaction.AWR() ),
  productions_( std::forward< std::vector< XS  > >( xs ) )
{ } catch( ... ){
  Log::info( "Trouble creating PhotonProductionReaction object "
             "from another PhotonProductionReaction." );
  throw;
}

PhotonProductionReaction() = delete;
