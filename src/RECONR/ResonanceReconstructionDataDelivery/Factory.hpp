class Factory {
  using Material_t = std::variant< ENDFMaterial_t >;

  Material_t material_;

public:
  #include "RECONR/ResonanceReconstructionDataDelivery/Factory/src/ctor.hpp"
  #include "RECONR/ResonanceReconstructionDataDelivery/Factory/src/call.hpp"
  #include "RECONR/ResonanceReconstructionDataDelivery/Factory/src/collectXS.hpp"
};
