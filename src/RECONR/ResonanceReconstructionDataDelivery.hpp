class ResonanceReconstructionDataDelivery {
public:
  using RawInterpolation_t = std::vector< interp::Variant >;
  using XSMap_t = std::map< ReactionID, Reaction >;
  using PPMap_t = std::map< ReactionID, PPReaction >;
  using ReconMap_t = std::map< ReactionID, std::vector< interp::LinearLinear > >;
  using UnresolvedMap_t = std::map< ReactionID, UnresolvedReaction >;

  using RPVariant = std::variant< ENDFtk::section::Type< 2, 151 > >;
  using Range_t = std::pair< double, double >;

  using Info_t = std::variant< ENDFtk::section::Type< 1, 451 > >;

private:
  elementary::ParticleID projectile_;
  elementary::ParticleID target_;

  Info_t information_;

  std::vector< int > MF3MTs_;

  XSMap_t reactions_;
  XSMap_t summations_;
  PPMap_t photonProductions_;
  ReconMap_t reconstructedResonances_;
  UnresolvedMap_t reconstructedUnresolved_;

  RPVariant resonanceParameters_;
  std::vector< double > resonanceReferenceGrid_;
  Range_t resolvedRange_;
  Range_t unresolvedRange_;
public:

  using Buffer_t = const std::string;
  using Tape_t = ENDFtk::syntaxTree::Tape< Buffer_t >;
  using ENDFMaterial_t = Tape_t::Material_t;

  #include "RECONR/ResonanceReconstructionDataDelivery/Factory.hpp"
  #include "RECONR/ResonanceReconstructionDataDelivery/src/getResonanceReferenceGrid.hpp"
  #include "RECONR/ResonanceReconstructionDataDelivery/src/ctor.hpp"

  auto& projectile() const { return this->projectile_; }
  auto& target() const { return this->target_; }

  auto& info() const { return this->information_; }

  const auto& MF3MTs() const { return this->MF3MTs_; }

  XSMap_t& reactions() { return this->reactions_; }
  const XSMap_t& reactions() const { return this->reactions_; }
  XSMap_t& summations() { return this->summations_; }
  const XSMap_t& summations() const { return this->summations_; }
  PPMap_t& photonProductions() { return this->photonProductions_; }
  const PPMap_t& photonProductions() const { return this->photonProductions_; }

  auto& reconstructedResonances() { return this->reconstructedResonances_; }
  auto& unresolved() { return this->reconstructedUnresolved_; }
  const auto& unresolved() const { return this->reconstructedUnresolved_; }

  const Range_t& resolvedRange() const { return this->resolvedRange_; }
  const Range_t& unresolvedRange() const { return this->unresolvedRange_; }

  const auto& resonanceParameters() const { return this->resonanceParameters_; }
  const auto& resonanceReferenceGrid() const { return this->resonanceReferenceGrid_; }
};

using R2D2 = ResonanceReconstructionDataDelivery;
