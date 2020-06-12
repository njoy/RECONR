#######################################################################
# Setup
#######################################################################

message( STATUS "Adding NJOY21 unit testing" )
enable_testing()


#######################################################################
# Unit testing directories
#######################################################################

add_subdirectory( src/RECONR/ReferenceGrid/test )
add_subdirectory( src/RECONR/ResonanceReconstructionDataDelivery/test )
add_subdirectory( src/RECONR/ResonanceReconstructionDataDelivery/Factory/test )
add_subdirectory( src/RECONR/RECONR/test )
