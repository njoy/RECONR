#ifndef RECONR_HPP
#define RECONR_HPP

#include <fstream>
#include <optional>


#include "range/v3/range_for.hpp"
#include "range/v3/action/push_back.hpp"
#include "range/v3/view/drop_while.hpp"
#include "range/v3/view/for_each.hpp"
#include "range/v3/view/generate.hpp"
#include "range/v3/view/reverse.hpp"
#include "range/v3/view/sliding.hpp"
#include "range/v3/view/unique.hpp"

#include "Log.hpp"
#include "nlohmann/json.hpp"

using namespace njoy;

#include "RECONR/ENDFtk.hpp"

#include "dimwits.hpp"
#include "resonanceReconstruction.hpp"
#include "interpolation.hpp"
#include "twig.hpp"
#include "constants.hpp"
#include "constants/CODATA2018.hpp"
#include "elementary.hpp"
#include "header-utilities/copy.hpp"
#include "header-utilities/slurpFileToMemory.hpp"

inline
auto CODATA = njoy::constants::CODATA2018;

namespace njoy   {

namespace utility{
#include "RECONR/src/sigfig.hpp"
} // namespace utility

namespace RECONR {

using ReactionID = elementary::ReactionID;
using Logger = std::pair< std::ostream&, std::ostream& >;

#include "RECONR/interp.hpp"

// For Reaction
using XSPair = std::pair< std::vector< double >, std::vector< double > >;
using XSForms = std::variant< 
  std::vector< interp::Variant >,
  interp::LinearTable,
  XSPair
>;

// For UnresolvedReaction
using UForms = std::variant< 
  interp::LinearLinear,
  XSPair
>;

// For PhotonProductions
using PPair = std::pair< double, double >;
using PPForms = std::variant<
  std::vector< interp::Variant >,
  interp::LinearTable,
  PPair
>;

// ForPhotonYields
using PYPair = std::pair< double, double >;
using PYForms = std::variant< 
  std::vector< interp::Variant >,
  interp::LinearTable,
  PYPair
>;

#include "RECONR/interp/src/MF12toInterpolation.hpp"
#include "RECONR/interp/src/MF13toInterpolation.hpp"

#include "RECONR/Reaction.hpp"
#include "RECONR/PhotonProduction.hpp"
#include "RECONR/PhotonYields.hpp"
#include "RECONR/UnresolvedReaction.hpp"
#include "RECONR/ReferenceGrid.hpp"
#include "RECONR/ResonanceReconstructionDataDelivery.hpp"
#include "RECONR/src/linearizeCriterion.hpp"
#include "RECONR/src/linearize.hpp"
#include "RECONR/RECONR.hpp"

}
}

#endif // RECONR_HPP
