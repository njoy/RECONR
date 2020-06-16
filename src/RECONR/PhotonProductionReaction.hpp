template< typename XS >
class PhotonProductionReaction {
  int ZA_;
  double atomicWeightRatio_;

  std::vector< XS > productions_;

public:
  #include "RECONR/PhotonProductionReaction/src/ctor.hpp"

  int ZA() const{ return this->ZA_; }
  double AWR() const{ return this->atomicWeightRatio_; }
  double atomicWeightRatio() const{ return this->AWR(); }

  const std::vector< XS >& productions() const{ return this->productions_; }
};

template< typename XS >
using PPReaction = PhotonProductionReaction< XS >;
