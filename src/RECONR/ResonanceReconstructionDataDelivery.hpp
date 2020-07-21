class ResonanceReconstructionDataDelivery {
public:
  using RawInterpolation_t = std::vector< interp::Variant >;
  using XSMap_t = std::map< ReactionID, Reaction >;
  using PPMap_t = std::map< ReactionID, PPReaction >;
  using ReconMap_t = std::map< ReactionID, std::vector< interp::LinearLinear > >;

  using RPVariant = std::variant< ENDFtk::section::Type< 2, 151 > >;

  using Info_t = std::variant< ENDFtk::section::Type< 1, 451 > >;

private:
  elementary::ParticleID projectile_;
  elementary::ParticleID target_;

  Info_t information_;

  XSMap_t reactions_;
  PPMap_t photonProductions_;
  ReconMap_t reconstructedResonances_;

  RPVariant resonanceParameters_;
  std::vector< double > resonanceReferenceGrid_;
public:

  using Buffer_t = const std::string;
  using Tape_t = ENDFtk::syntaxTree::Tape< Buffer_t >;
  using ENDFMaterial_t = Tape_t::Material_t;

  #include "RECONR/ResonanceReconstructionDataDelivery/Factory.hpp"
  #include "RECONR/ResonanceReconstructionDataDelivery/src/getResonanceReferenceGrid.hpp"
  #include "RECONR/ResonanceReconstructionDataDelivery/src/ctor.hpp"

  XSMap_t& reactions() { return this->reactions_; }
  const XSMap_t& reactions() const { return this->reactions_; }
  PPMap_t& photonProductions() { return this->photonProductions_; }
  const PPMap_t& photonProductions() const { return this->photonProductions_; }

  auto reconstructedResonances() const { 
    return this->reconstructedResonances_; }
  void reconstructedResonances( ReconMap_t&& rXS ) { 
    this->reconstructedResonances_ = rXS; }

  auto resonanceParameters() const { return this->resonanceParameters_; }
  auto resonanceReferenceGrid() const { return this->resonanceReferenceGrid_; }
};

using R2D2 = ResonanceReconstructionDataDelivery;
