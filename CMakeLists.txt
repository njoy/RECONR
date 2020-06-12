########################################################################
# Preamble
########################################################################

cmake_minimum_required( VERSION 3.16 )
project( RECONR LANGUAGES CXX )


########################################################################
# Project-wide setup
########################################################################

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED YES )

option( RECONR_unit_tests
    "Compile the RECONR unit tests and integrate with ctest" ON
    )


########################################################################
# Dependencies
########################################################################

include( cmake/RECONR_dependencies.cmake )


########################################################################
# Project targets
########################################################################

add_library( RECONR INTERFACE )
target_include_directories( RECONR INTERFACE src/ )
target_link_libraries( RECONR
    INTERFACE ENDFtk
    INTERFACE catch-adapter
    INTERFACE constants
    INTERFACE resonanceReconstruction
    INTERFACE Log
    INTERFACE twig
    INTERFACE json-adapter
    INTERFACE interpolation
    )


#######################################################################
# Top-level Only
#######################################################################

if( CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR )

    # unit testing
    if( RECONR_unit_tests )
        include( cmake/unit_testing.cmake )
    endif()

endif()
