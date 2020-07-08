class ResonanceReconstructionDataDelivery {
public:
  using RawInterpolation_t = std::vector< interp::Variant >;
  using XSMap_t = std::map< ReactionID, Reaction >;
  using PPMap_t = std::map< ReactionID, PPReaction< RawInterpolation_t > >;
  using PPLinMap_t = std::map< ReactionID, PPReaction< interp::LinearTable > >;
  using ReconMap_t = std::map< ReactionID, std::vector< interp::LinearLinear > >;

  using RPVariant = std::variant< ENDFtk::section::Type< 2, 151 > >;

private:
  XSMap_t reactions_;
  PPMap_t photonProductions_;
  PPLinMap_t linearPhotonProductions_;
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
  auto photonProductions() const { return this->photonProductions_; }

  auto linearPhotonProductions() const { return this->linearPhotonProductions_; }
  void linearPhotonProductions( PPLinMap_t&& ppMap ) { 
    this->linearPhotonProductions_ = ppMap; }

  auto reconstructedResonances() const { 
    return this->reconstructedResonances_; }
  void reconstructedResonances( ReconMap_t&& rXS ) { 
    this->reconstructedResonances_ = rXS; }

  auto resonanceParameters() const { return this->resonanceParameters_; }
  auto resonanceReferenceGrid() const { return this->resonanceReferenceGrid_; }
};

using R2D2 = ResonanceReconstructionDataDelivery;
