cmake_minimum_required( VERSION 3.14 )
include( FetchContent )

#######################################################################
# Declare project dependencies
#######################################################################

FetchContent_Declare( catch-adapter
    GIT_REPOSITORY  https://github.com/njoy/catch-adapter
    GIT_TAG         fb84b82ebf7a4789aa43cea560680cf745c6ee4f
    )

FetchContent_Declare( constants
    GIT_REPOSITORY  https://github.com/njoy/constants
    GIT_TAG         c113d6517a8946e0f4280ab5c38de50139df99d0
    )

FetchContent_Declare( dimwits
    GIT_REPOSITORY  https://github.com/njoy/DimensionalAnalysis
    GIT_TAG         acd794d373c8740a788f5c9d58a6eb8ba4d9861a
    )

FetchContent_Declare( disco
    GIT_REPOSITORY  https://github.com/njoy/disco
    GIT_TAG         a1a7ddb2c0f69465524d8640ee29988b714a881e
    )

FetchContent_Declare( eigen
    GIT_REPOSITORY  https://gitlab.com/libeigen/eigen.git
    GIT_TAG         dc252fbf00079ccab57948a164b1421703fe4361 # tag: 3.3.8
    )

FetchContent_Declare( elementary
    GIT_REPOSITORY  https://github.com/njoy/elementary
    GIT_TAG         97224cd76b92f174567fdd9259d3b7d8b41ccfae # tag: v0.2.2
    )

FetchContent_Declare( ENDFtk
    GIT_REPOSITORY  https://github.com/njoy/ENDFtk
    GIT_TAG         48aa8bcbc1f326b3d48bfa084047d05762eaea09
    )

FetchContent_Declare( hana-adapter
    GIT_REPOSITORY  https://github.com/njoy/hana-adapter
    GIT_TAG         f58e8973c9a614dc4f3720b5581a762c61bdbb40
    )

FetchContent_Declare( header-utilities
    GIT_REPOSITORY  https://github.com/njoy/header-utilities
    GIT_TAG         cc2610fee15e255c151e8e22aca1e8b3d1a96b39
    )

FetchContent_Declare( interpolation
    GIT_REPOSITORY  https://github.com/njoy/interpolation
    GIT_TAG         2a76934a148bf379ab594f6cdd2cdf4c8c28e447
    )

FetchContent_Declare( Log
    GIT_REPOSITORY  https://github.com/njoy/Log
    GIT_TAG         52962b7796afe37ef1d8f7edb4bf9ecb1b868d15
    )

FetchContent_Declare( nlohmann_json
    GIT_REPOSITORY  https://github.com/nlohmann/json
    GIT_TAG         e7b3b40b5a95bc74b9a7f662830a27c49ffc01b4 # tag: v3.7.3
    )

FetchContent_Declare( pybind11
    GIT_REPOSITORY  https://github.com/pybind/pybind11
    GIT_TAG         f1abf5d9159b805674197f6bc443592e631c9130 # tag: v2.6.1
    )

FetchContent_Declare( range-v3-adapter
    GIT_REPOSITORY  https://github.com/njoy/range-v3-adapter
    GIT_TAG         252679d4737c8f755d87c0e1eed6c37394a2ec59
    )

FetchContent_Declare( resonanceReconstruction
    GIT_REPOSITORY  https://github.com/njoy/resonanceReconstruction
    GIT_TAG         d54062b991c73e675f5afd2ba0db6c76eb2c2d9c
    )

FetchContent_Declare( spdlog
    GIT_REPOSITORY  https://github.com/gabime/spdlog
    GIT_TAG         a51b4856377a71f81b6d74b9af459305c4c644f8
    )

FetchContent_Declare( twig
    GIT_REPOSITORY  https://github.com/njoy/twig
    GIT_TAG         e209d801a1dc99644ee48e9caaf0afb07ef982ff
    )

#######################################################################
# Load dependencies
#######################################################################

FetchContent_MakeAvailable(
    catch-adapter
    constants
    dimwits
    disco
    eigen
    elementary
    ENDFtk
    hana-adapter
    header-utilities
    interpolation
    Log
    nlohmann_json
    pybind11
    range-v3-adapter
    resonanceReconstruction
    spdlog
    twig
    )
