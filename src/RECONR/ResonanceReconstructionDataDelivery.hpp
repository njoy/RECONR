class ResonanceReconstructionDataDelivery {
  using buffer_t = std::string;
  using Tape_t = ENDFtk::syntaxTree::Tape< buffer_t >;
  using ENDFMaterial_t = std::decay_t< decltype( Tape_t{ std::string{} }.materialNumber( 1 ).front() ) >;

  using Material_t = std::variant< ENDFMaterial_t >;
  Material_t material;
  using XSmap_t = std::map< int, interp::Table >;
  XSmap_t crossSections_;

public:
  #include "RECONR/ResonanceReconstructionDataDelivery/src/collectXS.hpp"
  #include "RECONR/ResonanceReconstructionDataDelivery/src/ctor.hpp"

  auto crossSections() const{ return this->crossSections_; }
};

using R2D2 = ResonanceReconstructionDataDelivery;
