class RECONR{

public:
  using buffer_t = decltype( njoy::utility::slurpFileToMemory( "" ) );
  using Tape_t = std::decay_t< decltype( 
      ENDFtk::syntaxTree::makeTape( buffer_t{} ) ) >;
  using Material_t = ENDFtk::syntaxTree::Material< buffer_t >;

  const nlohmann::json njoyArgs;
  const nlohmann::json args;


protected:
  using Format_t = std::variant< Tape_t >;
  Format_t evaluatedData;

  #include "RECONR/RECONR/src/getEvaluated.hpp"

public:
  #include "RECONR/RECONR/src/ctor.hpp"
  #include "RECONR/RECONR/src/processMaterial.hpp"
  #include "RECONR/RECONR/src/call.hpp"
};
