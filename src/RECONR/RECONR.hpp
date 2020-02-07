class RECONR{

public:
  using buffer_t = std::string;
  using Tape_t = ENDFtk::syntaxTree::Tape< buffer_t >;
  using Material_t = std::decay_t< decltype( Tape_t{ std::string{} }.materialNumber( 1 ).front() ) >;
  // using Material_t = ENDFtk::syntaxTree::Material< const buffer_t::iterator >;

protected:
  using Format_t = std::variant< Tape_t >;


public:
  #include "RECONR/RECONR/src/getEvaluated.hpp"
  // #include "RECONR/RECONR/src/findR2D2.hpp"
  #include "RECONR/RECONR/src/call.hpp"
};
