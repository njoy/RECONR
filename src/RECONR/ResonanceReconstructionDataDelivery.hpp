class ResonanceReconstructionDataDelivery {
  using XSmap_t = std::map< int, std::vector< interp::Variant > >;
  XSmap_t crossSections_;

public:
  using LinMap_t = std::map< int, interp::LinearTable >;

  using Buffer_t = std::string;
  using Tape_t = ENDFtk::syntaxTree::Tape< Buffer_t >;
  using ENDFMaterial_t = std::decay_t< 
    decltype( Tape_t{ std::string{} }.materialNumber( 1 ).front() ) >;

  #include "RECONR/ResonanceReconstructionDataDelivery/Factory.hpp"
  #include "RECONR/ResonanceReconstructionDataDelivery/src/ctor.hpp"

  auto crossSections() const{ return this->crossSections_; }
  auto at( int i ){ return crossSections_.at( i ); }
};

using R2D2 = ResonanceReconstructionDataDelivery;
