class ResonanceReconstructionDataDelivery {
public:
  using buffer_t = std::string;
  using Tape_t = ENDFtk::syntaxTree::Tape< buffer_t >;
  using ENDFMaterial_t = std::decay_t< 
    decltype( Tape_t{ std::string{} }.materialNumber( 1 ).front() ) >;
  using Material_t = std::variant< ENDFMaterial_t >;
  using XSmap_t = std::map< int, interp::Table >;

private:
  Material_t material;
  XSmap_t crossSections_;

public:
  #include "RECONR/ResonanceReconstructionDataDelivery/src/collectXS.hpp"
  #include "RECONR/ResonanceReconstructionDataDelivery/src/ctor.hpp"

  auto crossSections() const{ return this->crossSections_; }
};

using R2D2 = ResonanceReconstructionDataDelivery;
