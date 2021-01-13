class UnresolvedReaction {
private:
  int ZA_;
  double atomicWeightRatio_;
  bool lssf_;

  UForms crossSection_;

public:
  #include "RECONR/UnresolvedReaction/src/ctor.hpp"

  int ZA() const { return this->ZA_; }
  double AWR() const { return this->atomicWeightRatio_; }
  bool LSSF() const { return lssf_; }

  #include "RECONR/UnresolvedReaction/src/crossSections.hpp"
};

using URxn = UnresolvedReaction;
