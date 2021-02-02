cmake_minimum_required( VERSION 3.14 )
include( FetchContent )

#######################################################################
# Declare project dependencies
#######################################################################

FetchContent_Declare( nlohmann_json
    GIT_REPOSITORY  https://github.com/nlohmann/json
    GIT_TAG         v3.7.3
    )
set(JSON_BuildTests OFF CACHE INTERNAL "")

FetchContent_Declare( ENDFtk
    GIT_REPOSITORY  https://github.com/njoy/ENDFtk
    GIT_TAG         origin/develop
    GIT_SHALLOW     TRUE
    )

FetchContent_Declare( catch-adapter
    GIT_REPOSITORY  https://github.com/njoy/catch-adapter
    GIT_TAG         origin/master
    GIT_SHALLOW     TRUE
    )

FetchContent_Declare( constants
    GIT_REPOSITORY  https://github.com/njoy/constants
    GIT_TAG         origin/master
    GIT_SHALLOW     TRUE
    )

FetchContent_Declare( resonanceReconstruction
    GIT_REPOSITORY  https://github.com/njoy/resonanceReconstruction
    GIT_TAG         origin/develop
    GIT_SHALLOW     TRUE
    )

FetchContent_Declare( Log
    GIT_REPOSITORY  https://github.com/njoy/Log
    GIT_TAG         origin/master
    GIT_SHALLOW     TRUE
    )

FetchContent_Declare( twig
    GIT_REPOSITORY  https://github.com/njoy/twig
    GIT_TAG         origin/master
    GIT_SHALLOW     TRUE
    )

FetchContent_Declare( interpolation
    GIT_REPOSITORY  https://github.com/njoy/interpolation
    GIT_TAG         origin/master
    GIT_SHALLOW     TRUE
    )

FetchContent_Declare( elementary
    GIT_REPOSITORY  https://github.com/njoy/elementary
    GIT_TAG         origin/master
    GIT_SHALLOW     TRUE
    )

#######################################################################
# Load dependencies
#######################################################################

FetchContent_MakeAvailable(
    ENDFtk
    catch-adapter
    constants
    resonanceReconstruction
    Log
    twig
    nlohmann_json
    interpolation
    elementary
    )
