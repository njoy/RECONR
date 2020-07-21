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
    GIT_TAG         origin/build/fetchcontent-lrf7
    )

FetchContent_Declare( Log
    GIT_REPOSITORY  http://github.com/njoy/Log
    GIT_TAG         origin/build/fetchcontent
    )

FetchContent_Declare( twig
    GIT_REPOSITORY  http://github.com/njoy/twig
    GIT_TAG         origin/build/fetchcontent
    )

FetchContent_Declare(json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.7.3
    GIT_SHALLOW true
    )

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_Declare( interpolation
    GIT_REPOSITORY  http://github.com/njoy/interpolation
    GIT_TAG         origin/build/fetchcontent
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
    json
    interpolation
    elementary
    )
