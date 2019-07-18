add_compile_definitions(S_FUNCTION_NAME=ParameterExample)
file(GLOB TEST_SOURCES *.cpp *.c)
add_compile_definitions(DEBUG)
#add_compile_definitions(DEBUG_VERBOSITY_LIFECYCLE)
add_compile_definitions(DEBUG_SIMULINK_CALLBACK)

set(CMAKE_BUILD_TYPE Debug)
include_directories(${SIMEX_INCLUDES})
matlab_add_mex(
        NAME ParameterExample
        SRC ${SIMEX_SOURCES} ${TEST_SOURCES}
        LINK_TO ${Matlab_MEX_LIBRARY}
)