class ResonanceReconstructionDataDelivery {
public:
  using XSmap_t = std::map< int, std::vector< interp::Variant > >;
  using LinMap_t = std::map< int, interp::LinearTable >;

  using RPVariant = std::variant< 
    std::vector< ENDFtk::resonanceParameters::Isotope >
  >;

private:
  XSmap_t crossSections_;
  LinMap_t linearCrossSections_;

  RPVariant resonanceParameters_;
  std::vector< double > resonanceReferenceGrid_;
public:

  using Buffer_t = std::string;
  using Tape_t = ENDFtk::syntaxTree::Tape< Buffer_t >;
  using ENDFMaterial_t = std::decay_t< 
    decltype( Tape_t{ std::string{} }.materialNumber( 1 ).front() ) >;

  #include "RECONR/ResonanceReconstructionDataDelivery/Factory.hpp"
  #include "RECONR/ResonanceReconstructionDataDelivery/src/getResonanceReferenceGrid.hpp"
  #include "RECONR/ResonanceReconstructionDataDelivery/src/ctor.hpp"

  auto crossSections() const { return this->crossSections_; }
  auto at( int i ){ return crossSections_.at( i ); }

  auto linearCrossSections() const { return this->linearCrossSections_; }
  void linearCrossSections( LinMap_t&& lXS ){ 
    this-> linearCrossSections_ = lXS; }

  auto resonanceParameters() const { return this->resonanceParameters_; }

  auto resonanceReferenceGrid() const { return this->resonanceReferenceGrid_; }
};

using R2D2 = ResonanceReconstructionDataDelivery;
