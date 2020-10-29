#######################################################################
# Setup
#######################################################################

message( STATUS "Adding RECONR unit testing" )
enable_testing()


#######################################################################
# Unit testing directories
#######################################################################

add_subdirectory( src/RECONR/RECONR/test )
add_subdirectory( src/RECONR/ReferenceGrid/test )
