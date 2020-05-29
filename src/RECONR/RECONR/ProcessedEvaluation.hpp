class ProcessedEvaluation{
  Format_t& evaluated;
  std::ofstream pendf;
  std::ostreambuf_iterator< char > ipendf;

public:
  #include "RECONR/RECONR/ProcessedEvaluation/src/ctor.hpp"
  #include "RECONR/RECONR/ProcessedEvaluation/src/header.hpp"
  #include "RECONR/RECONR/ProcessedEvaluation/src/footer.hpp"
  #include "RECONR/RECONR/ProcessedEvaluation/src/material.hpp"

};
