class PhotonProduction{
private:
  int ZA_;
  double atomicWeightRatio_;

  std::vector< PPForms > productions_;

public:
  #include "RECONR/PhotonProduction/src/ctor.hpp"

  int ZA() const{ return this->ZA_; }
  double AWR() const{ return this->atomicWeightRatio_; }
  double atomicWeightRatio() const{ return this->AWR(); }

  #include "RECONR/PhotonProduction/src/productions.hpp"
};

using PPReaction = PhotonProduction;
