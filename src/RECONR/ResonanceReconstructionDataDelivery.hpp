class ResonanceReconstructionDataDelivery {
public:
  using Key_t = int;
  using RawInterpolation_t = std::vector< interp::Variant >;
  using XSMap_t = std::map< Key_t, Reaction< RawInterpolation_t > >;
  using PPMap_t = std::map< Key_t, 
    PPReaction< std::vector< RawInterpolation_t > > >;
  using LinMap_t = std::map< Key_t, Reaction< interp::LinearTable > >;
  using PPLinMap_t = std::map< Key_t, PPReaction< interp::LinearTable > >;
  using ReconMap_t = std::map< Key_t, std::vector< interp::LinearLinear > >;

  using RPVariant = std::variant< ENDFtk::section::Type< 2, 151 > >;

private:
  XSMap_t reactions_;
  PPMap_t photonProductions_;
  LinMap_t linearReactions_;
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

  auto reactions() const { return this->reactions_; }
  auto photonProductions() const { return this->photonProductions_; }

  auto linearReactions() const { return this->linearReactions_; }
  void linearReactions( LinMap_t&& lXS ){ this->linearReactions_ = lXS; }
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
