class ResonanceReconstructionDataDelivery {
  using Material_t = ENDFtk::syntaxTree::Material< std::string::iterator >;

  Material_t material;
  using XSmap_t = std::map< int, ENDF::Tab1 >;
  XSmap_t crossSections_;

public:
  #include "RECONR/ResonanceReconstructionDataDelivery/src/collectXS.hpp"
  #include "RECONR/ResonanceReconstructionDataDelivery/src/ctor.hpp"

  auto crossSections() const{ return this->crossSections_; }
};

using R2D2 = ResonanceReconstructionDataDelivery;
