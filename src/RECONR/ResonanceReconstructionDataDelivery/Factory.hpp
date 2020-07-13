class Factory {
  using Material_t = std::variant< ENDFMaterial_t >;

  #include "RECONR/ResonanceReconstructionDataDelivery/Factory/src/collectXS.hpp"
  #include "RECONR/ResonanceReconstructionDataDelivery/Factory/src/collectPPXS.hpp"
  #include "RECONR/ResonanceReconstructionDataDelivery/Factory/src/collectRP.hpp"

public:
  #include "RECONR/ResonanceReconstructionDataDelivery/Factory/src/call.hpp"
};
