#ifndef RECONR_HPP
#define RECONR_HPP

#include <fstream>

#include "json.hpp"

#include "ENDFtk.hpp"
#include "resonanceReconstruction.hpp"
#include "interpolation.hpp"
#include "twig.hpp"

namespace njoy   {
namespace RECONR {

using namespace ENDFtk::resonanceParameters;

#include "RECONR/interp.hpp"
#include "RECONR/Reaction.hpp"
#include "RECONR/ReferenceGrid.hpp"
#include "RECONR/ResonanceReconstructionDataDelivery.hpp"
#include "RECONR/src/linearize.hpp"
#include "RECONR/RECONR.hpp"

}
}

#endif // RECONR_HPP
