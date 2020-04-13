class RECONR{

public:
  using buffer_t = std::string;
  using Tape_t = ENDFtk::syntaxTree::Tape< buffer_t >;
  using Material_t = ENDFtk::syntaxTree::Material< std::string::iterator >;

protected:
  using Format_t = std::variant< Tape_t >;
  using Map_t = std::map< int, R2D2 >;

public:
  #include "RECONR/RECONR/src/linearize.hpp"
  #include "RECONR/RECONR/src/getEvaluated.hpp"
  #include "RECONR/RECONR/src/findR2D2.hpp"
  #include "RECONR/RECONR/src/call.hpp"
};
