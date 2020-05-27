template< typename XS >
class Reaction {
  int ZA_;
  double atomicWeightRatio_;
  double massDifferenceQ_;
  double reactionQ_;
  int complexBreakUp_;

  XS crossSection_;

public:
  #include "RECONR/Reaction/src/ctor.hpp"

  int ZA() const{ return this->ZA_; }
  double AWR() const{ return this->atomicWeightRatio_; }
  double atomicWeightRatio() const{ return this->AWR(); }
  double QM() const { return this->massDifferenceQ_; }
  double massDifferenceQValue() const { return this->QM(); }
  double QI() const { return this->reactionQ_; }
  double reactionQValue() const { return this->QI(); }
  int LR() const { return this->complexBreakUp_; }
  int complexBreakUp() const { return this->LR(); }

  const XS& crossSections() const{ return this->crossSection_; }

};
