class ResonanceReconstructionDataDelivery {
public:

#include "RECONR/ResonanceReconstructionDataDelivery/Factory.hpp"
  using XSmap_t = std::map< int, interp::Table >;

private:
  Material_t material;
  XSmap_t crossSections_;

public:
  #include "RECONR/ResonanceReconstructionDataDelivery/src/ctor.hpp"

  auto crossSections() const{ return this->crossSections_; }
};

using R2D2 = ResonanceReconstructionDataDelivery;
