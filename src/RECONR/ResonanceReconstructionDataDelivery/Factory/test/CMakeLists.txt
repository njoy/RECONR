
add_executable( RECONR.ResonanceReconstructionDataDelivery.Factory.test
                collectRP.test.cpp
                Factory.test.cpp
                collectXS.test.cpp )
target_compile_options( RECONR.ResonanceReconstructionDataDelivery.Factory.test PRIVATE ${${PREFIX}_common_flags}
$<$<BOOL:${strict}>:${${PREFIX}_strict_flags}>$<$<CONFIG:DEBUG>:
${${PREFIX}_DEBUG_flags}
$<$<BOOL:${coverage}>:${${PREFIX}_coverage_flags}>>
$<$<CONFIG:RELEASE>:
${${PREFIX}_RELEASE_flags}
$<$<BOOL:${link_time_optimization}>:${${PREFIX}_link_time_optimization_flags}>
$<$<BOOL:${nonportable_optimization}>:${${PREFIX}_nonportable_optimization_flags}>>

${CXX_appended_flags} ${reconr_appended_flags} )
target_link_libraries( RECONR.ResonanceReconstructionDataDelivery.Factory.test PUBLIC reconr ) 
add_test( NAME RECONR.ResonanceReconstructionDataDelivery.Factory COMMAND RECONR.ResonanceReconstructionDataDelivery.Factory.test )