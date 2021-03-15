#include "ENDFtk/tree/Tape.hpp"
#include "ENDFtk/tree/fromFile.hpp"
#include "ENDFtk/file/1.hpp"
#include "ENDFtk/file/2.hpp"
#include "ENDFtk/file/3.hpp"
#include "ENDFtk/file/12.hpp"
#include "ENDFtk/file/13.hpp"
#include "ENDFtk/isRedundant.hpp"

namespace RP {

  using ScatteringRadius = njoy::ENDFtk::section::Type< 2, 151 >::ScatteringRadius;
  using ResonanceRange = njoy::ENDFtk::section::Type< 2, 151 >::ResonanceRange;
  using BreitWignerLValue = njoy::ENDFtk::section::Type< 2, 151 >::BreitWignerLValue;
  using ReichMooreLValue = njoy::ENDFtk::section::Type< 2, 151 >::ReichMooreLValue;
  using Isotope = njoy::ENDFtk::section::Type< 2, 151 >::Isotope;
  using SpecialCase = njoy::ENDFtk::section::Type< 2, 151 >::SpecialCase;
  using SLBW = njoy::ENDFtk::section::Type< 2, 151 >::SingleLevelBreitWigner;
  using MLBW = njoy::ENDFtk::section::Type< 2, 151 >::MultiLevelBreitWigner;
  using RM = njoy::ENDFtk::section::Type< 2, 151 >::ReichMoore;
  using RML = njoy::ENDFtk::section::Type< 2, 151 >::RMatrixLimited;
  using CaseA = njoy::ENDFtk::section::Type< 2, 151 >::CaseA;
  using CaseB = njoy::ENDFtk::section::Type< 2, 151 >::CaseB;
  using CaseC = njoy::ENDFtk::section::Type< 2, 151 >::CaseC;
}
