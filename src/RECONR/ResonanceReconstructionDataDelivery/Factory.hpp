class Factory {
  using Buffer_t = std::string;
  using Tape_t = ENDFtk::syntaxTree::Tape< Buffer_t >;
  using ENDFMaterial_t = std::decay_t< 
    decltype( Tape_t{ std::string{} }.materialNumber( 1 ).front() ) >;
  using Material_t = std::variant< ENDFMaterial_t >;

public:
  #include "RECONR/ResonanceReconstructionDataDelivery/src/ctor.hpp"
  #include "RECONR/ResonanceReconstructionDataDelivery/src/collectXS.hpp"
};
