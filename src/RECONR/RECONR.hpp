class RECONR{

public:
  using buffer_t = decltype( njoy::utility::slurpFileToMemory( "" ) );
  using Tape_t = std::decay_t< decltype( 
      ENDFtk::syntaxTree::makeTape( buffer_t{} ) ) >;

  const nlohmann::json njoyArgs;
  const nlohmann::json args;


protected:
  using Format_t = std::variant< Tape_t >;
  Format_t evaluated;

  #include "RECONR/RECONR/src/getEvaluated.hpp"
public:
  #include "RECONR/RECONR/src/ctor.hpp"
};
