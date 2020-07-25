#ifndef RECONR_HPP
#define RECONR_HPP

#include <fstream>
#include <optional>

#include "nlohmann/json.hpp"

#include "ENDFtk.hpp"
#include "dimwits.hpp"
#include "resonanceReconstruction.hpp"
#include "interpolation.hpp"
#include "twig.hpp"
#include "constants.hpp"
#include "constants/CODATA2018.hpp"
#include "elementary.hpp"

inline
auto CODATA = njoy::constants::CODATA2018;

namespace njoy   {
namespace RECONR {

using namespace ENDFtk::resonanceParameters;

using ReactionID = elementary::ReactionID;

#include "RECONR/interp.hpp"

using XSPair = std::pair< std::vector< double >, std::vector< double > >;
using XSForms = std::variant< 
  std::vector< interp::Variant >,
  interp::LinearTable,
  XSPair
>;
using PPair = std::pair< double, double >;
using PPForms = std::variant<
  std::vector< interp::Variant >,
  interp::LinearTable,
  PPair
>;

#include "RECONR/interp/src/MF13toInterpolation.hpp"

#include "RECONR/Reaction.hpp"
#include "RECONR/PhotonProduction.hpp"
#include "RECONR/ReferenceGrid.hpp"
#include "RECONR/ResonanceReconstructionDataDelivery.hpp"
#include "RECONR/src/linearize.hpp"
#include "RECONR/RECONR.hpp"

}
}

#endif // RECONR_HPP
