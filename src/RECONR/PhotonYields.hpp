class PhotonYields{
private:
  int ZA_;
  double atomicWeightRatio_;

  std::vector< PYForms > yields_;
public:
  #include "RECONR/PhotonYields/src/ctor.hpp"

  int ZA() const{ return this->ZA_; }
  double AWR() const{ return this->atomicWeightRatio_; }
  double atomicWeightRatio() const{ return this->AWR(); }

  #include "RECONR/PhotonYields/src/yields.hpp"
};
