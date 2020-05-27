class RECONR{

public:
  using buffer_t = std::string;
  using Tape_t = ENDFtk::syntaxTree::Tape< buffer_t >;
  using Material_t = ENDFtk::syntaxTree::Material< std::string::iterator >;

  static constexpr double absoluteTolerance{ 1E-10 };

  #include "RECONR/RECONR/src/storeBWReconstruction.hpp"
  #include "RECONR/RECONR/src/sumPartials.hpp"

protected:
  using Format_t = std::variant< Tape_t >;

public:
  #include "RECONR/RECONR/src/findR2D2.hpp"
  #include "RECONR/RECONR/src/getEvaluated.hpp"
  #include "RECONR/RECONR/src/linearizeXS.hpp"
  #include "RECONR/RECONR/src/unionizeEnergyGrid.hpp"
  #include "RECONR/RECONR/src/reconstructResonances.hpp"
  #include "RECONR/RECONR/src/summateReactions.hpp"
  #include "RECONR/RECONR/src/call.hpp"
};
