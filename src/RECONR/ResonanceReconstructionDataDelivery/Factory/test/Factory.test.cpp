#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "range/v3/algorithm/equal.hpp"

#include "RECONR.hpp"

#include "RECONR/details/simpleENDFTestString.hpp"
#include "RECONR/details/interpLambdas.hpp"
#include "RECONR/details/checkRanges.hpp"
#include "RECONR/details/printV.hpp"

using namespace njoy;
using namespace njoy::RECONR;
using namespace njoy::elementary;

class TFactory: protected njoy::RECONR::R2D2::Factory {
  using Factory = njoy::RECONR::R2D2::Factory;

public:
  using Factory::Factory;
  using Factory::information;
  using Factory::collectRP;
  using Factory::collectXS;
  using Factory::collectPPXS;
  using Factory::collectResonanceRanges;
};

njoy::RECONR::Logger logger{ std::cout, std::cerr };

#include "RECONR/ResonanceReconstructionDataDelivery/Factory/test/collectXS.hpp"
#include "RECONR/ResonanceReconstructionDataDelivery/Factory/test/collectPPXS.hpp"
#include "RECONR/ResonanceReconstructionDataDelivery/Factory/test/collectRP.hpp"
