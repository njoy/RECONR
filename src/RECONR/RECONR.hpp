class RECONR{

public:
  using buffer_t = decltype( njoy::utility::slurpFileToMemory( "" ) );
  using Tape_t = ENDFtk::syntaxTree::Tape< buffer_t >;
  using Material_t = ENDFtk::syntaxTree::Material< buffer_t >;

protected:
  using Format_t = std::variant< Tape_t >;


public:
  #include "RECONR/RECONR/src/getEvaluated.hpp"
  #include "RECONR/RECONR/src/collectData.hpp"
  #include "RECONR/RECONR/src/call.hpp"
};
