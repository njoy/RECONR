include( FetchContent )


########################################################################
# Forward declarations
########################################################################


########################################################################
# Declare project dependencies
########################################################################

FetchContent_Declare( ENDFtk
    GIT_REPOSITORY  http://github.com/njoy/ENDFtk
    GIT_TAG         origin/master
    )

FetchContent_Declare( catch-adapter
    GIT_REPOSITORY  http://github.com/njoy/catch-adapter
    GIT_TAG         origin/master
    )

FetchContent_Declare( constants
    GIT_REPOSITORY  http://github.com/njoy/constants
    GIT_TAG         origin/master
    )

FetchContent_Declare( resonanceReconstruction
    GIT_REPOSITORY  http://github.com/njoy/resonanceReconstruction
    GIT_TAG         origin/master
    )

FetchContent_Declare( Log
    GIT_REPOSITORY  http://github.com/njoy/Log
    GIT_TAG         origin/master
    )

FetchContent_Declare( twig
    GIT_REPOSITORY  http://github.com/njoy/twig
    GIT_TAG         origin/master
    )

FetchContent_Declare( header-utilities
    GIT_REPOSITORY  http://github.com/njoy/header-utilities
    GIT_TAG         origin/master
    )

FetchContent_Declare( nlohmann_json
    GIT_REPOSITORY  https://github.com/nlohmann/json
    GIT_TAG         v3.7.3
    )
set(JSON_BuildTests OFF CACHE INTERNAL "")

FetchContent_Declare( interpolation
    GIT_REPOSITORY  http://github.com/njoy/interpolation
    GIT_TAG         origin/master
    )

FetchContent_Declare( elementary
    GIT_REPOSITORY  http://github.com/njoy/elementary
    GIT_TAG         origin/master
    )


########################################################################
# Load dependencies
########################################################################

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
