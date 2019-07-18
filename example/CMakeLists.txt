project(SimexExample)
cmake_minimum_required(VERSION 3.14)

add_compile_definitions(S_FUNCTION_NAME=SimexExample)


include(../CMakeLists.txt)
include_directories(${SIMEX_INCLUDES})

set(SOURCES SimexExampleBlock.cpp)

set(CMAKE_BUILD_TYPE Debug)
matlab_add_mex(
        NAME SimexExample
        SRC ${SIMEX_SOURCES} ${SOURCES}
        LINK_TO ${Matlab_MEX_LIBRARY}
)