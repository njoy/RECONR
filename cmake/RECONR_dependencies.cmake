include( FetchContent )


########################################################################
# Forward declarations
########################################################################


########################################################################
# Declare project dependencies
########################################################################

FetchContent_Declare( ENDFtk
    GIT_REPOSITORY  http://github.com/njoy/ENDFtk
    GIT_TAG         origin/build/fetchcontent
    )

FetchContent_Declare( catch-adapter
    GIT_REPOSITORY  http://github.com/njoy/catch-adapter
    GIT_TAG         origin/build/fetchcontent
    )

FetchContent_Declare( constants
    GIT_REPOSITORY  http://github.com/njoy/constants
    GIT_TAG         origin/build/fetchcontent
    )

FetchContent_Declare( resonanceReconstruction
    GIT_REPOSITORY  http://github.com/njoy/resonanceReconstruction
    GIT_TAG         origin/build/fetchcontent
    )

FetchContent_Declare( Log
    GIT_REPOSITORY  http://github.com/njoy/Log
    GIT_TAG         origin/build/fetchcontent
    )

FetchContent_Declare( twig
    GIT_REPOSITORY  http://github.com/njoy/twig
    GIT_TAG         origin/build/fetchcontent
    )

FetchContent_Declare( json-adapter
    GIT_REPOSITORY  http://github.com/njoy/json-adapter
    GIT_TAG         origin/build/fetchcontent
    )

FetchContent_Declare( interpolation
    GIT_REPOSITORY  http://github.com/njoy/interpolation
    GIT_TAG         origin/build/fetchcontent
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
    json-adapter
    interpolation
    )
