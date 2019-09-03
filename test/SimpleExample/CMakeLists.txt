project(SimpleExample)
set(FUNCTION_NAME SimpleExample)
add_compile_definitions(S_FUNCTION_NAME=${FUNCTION_NAME})
file(GLOB TEST_SOURCES *.cpp *.c)

set(CMAKE_BUILD_TYPE Debug)
include_directories(${SIMEX_INCLUDE_DIRS})

matlab_add_mex(
        NAME ${FUNCTION_NAME}
        SRC ${SIMEX_SOURCES} ${TEST_SOURCES}
        LINK_TO ${SIMEX_LIBS}
)