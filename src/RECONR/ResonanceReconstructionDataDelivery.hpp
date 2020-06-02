class ResonanceReconstructionDataDelivery {
public:
  using Key_t = int;
  using XSmap_t = std::map< Key_t, Reaction< std::vector< interp::Variant > > >;
  using LinMap_t = std::map< Key_t, Reaction< interp::LinearTable > >;
  using ReconMap_t = std::map< Key_t, std::vector< interp::LinearLinear > >;

  using RPVariant = std::variant< ENDFtk::section::Type< 2, 151 > >;

private:
  XSmap_t reactions_;
  LinMap_t linearReactions_;
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
  auto at( int i ){ return reactions_.at( i ); }

  auto linearReactions() const { return this->linearReactions_; }
  void linearReactions( LinMap_t&& lXS ){ this->linearReactions_ = lXS; }

  auto reconstructedResonances() const { 
    return this->reconstructedResonances_; }
  void reconstructedResonances( ReconMap_t&& rXS ) { 
    this->reconstructedResonances_ = rXS; }

  auto resonanceParameters() const { return this->resonanceParameters_; }

  auto resonanceReferenceGrid() const { return this->resonanceReferenceGrid_; }
};

using R2D2 = ResonanceReconstructionDataDelivery;
