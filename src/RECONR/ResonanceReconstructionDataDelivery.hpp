class ResonanceReconstructionDataDelivery {
  using Buffer_t = decltype( njoy::utility::slurpFileToMemory( "" ) );
  using Tape_t = std::decay_t< 
    decltype( ENDFtk::syntaxTree::makeTape( Buffer_t{} ) ) >;
  // using Material_t = ENDFtk::syntaxTree::Material< Buffer_t >;
  using Material_t = decltype( 
      ENDFtk::syntaxTree::makeTape( Buffer_t{} ).materialNumber( 1 ).front() );

  Material_t material;
  using XSmap_t = std::map< int, ENDF::Tab1 >;
  XSmap_t crossSections_;

public:
  #include "RECONR/ResonanceReconstructionDataDelivery/src/collectXS.hpp"
  #include "RECONR/ResonanceReconstructionDataDelivery/src/ctor.hpp"

  auto crossSections() const{ return this->crossSections_; }
};

using R2D2 = ResonanceReconstructionDataDelivery;
