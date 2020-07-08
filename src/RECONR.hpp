#ifndef RECONR_HPP
#define RECONR_HPP

#include <fstream>

#include "json.hpp"

#include "ENDFtk.hpp"
#include "dimwits.hpp"
#include "resonanceReconstruction.hpp"
#include "interpolation.hpp"
#include "twig.hpp"
// #include "constants.hpp"
// #include "constants/CODATA2018.hpp"

// auto CODATA = njoy::constants::CODATA2018;

namespace njoy   {
namespace RECONR {

using namespace ENDFtk::resonanceParameters;

using ReactionID = std::string; // Attribution to @whaeck

#include "RECONR/src/MT2ReactionID.hpp"
#include "RECONR/src/reactionID2MT.hpp"
#include "RECONR/interp.hpp"
#include "RECONR/Reaction.hpp"
#include "RECONR/PhotonProductionReaction.hpp"
#include "RECONR/ReferenceGrid.hpp"
#include "RECONR/ResonanceReconstructionDataDelivery.hpp"
#include "RECONR/src/linearize.hpp"
#include "RECONR/RECONR.hpp"

}
}

#endif // RECONR_HPP
