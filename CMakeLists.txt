########################################################################
# Preamble
########################################################################

cmake_minimum_required( VERSION 3.16 )
project( RECONR LANGUAGES CXX )

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
