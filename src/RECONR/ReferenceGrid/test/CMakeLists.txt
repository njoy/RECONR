
add_executable( RECONR.ReferenceGrid.test
                call.test.cpp
                ReferenceGrid.test.cpp )
target_compile_options( RECONR.ReferenceGrid.test PRIVATE ${${PREFIX}_common_flags}
$<$<BOOL:${strict}>:${${PREFIX}_strict_flags}>$<$<CONFIG:DEBUG>:
${${PREFIX}_DEBUG_flags}
$<$<BOOL:${coverage}>:${${PREFIX}_coverage_flags}>>
$<$<CONFIG:RELEASE>:
${${PREFIX}_RELEASE_flags}
$<$<BOOL:${link_time_optimization}>:${${PREFIX}_link_time_optimization_flags}>
$<$<BOOL:${nonportable_optimization}>:${${PREFIX}_nonportable_optimization_flags}>>

${CXX_appended_flags} ${RECONR_appended_flags} )
target_link_libraries( RECONR.ReferenceGrid.test PUBLIC RECONR ) 
add_test( NAME RECONR.ReferenceGrid COMMAND RECONR.ReferenceGrid.test )