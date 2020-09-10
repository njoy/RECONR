class Factory {
  using Material_t = std::variant< ENDFMaterial_t >;

protected:
  #include "RECONR/ResonanceReconstructionDataDelivery/Factory/src/information.hpp"
  #include "RECONR/ResonanceReconstructionDataDelivery/Factory/src/target.hpp"
  #include "RECONR/ResonanceReconstructionDataDelivery/Factory/src/MTs.hpp"
  #include "RECONR/ResonanceReconstructionDataDelivery/Factory/src/projectile.hpp"
  #include "RECONR/ResonanceReconstructionDataDelivery/Factory/src/collectXS.hpp"
  #include "RECONR/ResonanceReconstructionDataDelivery/Factory/src/collectPPXS.hpp"
  #include "RECONR/ResonanceReconstructionDataDelivery/Factory/src/collectRP.hpp"
  #include "RECONR/ResonanceReconstructionDataDelivery/Factory/src/collectResonanceRanges.hpp"

public:
  #include "RECONR/ResonanceReconstructionDataDelivery/Factory/src/call.hpp"
};
