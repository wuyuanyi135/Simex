project(IPCBlockExample)

set(CMAKE_BUILD_TYPE Debug)
include_directories(${SIMEX_INCLUDE_DIRS})

set(FUNCTION_NAME IPCBlockExample)
matlab_add_mex(
        NAME ${FUNCTION_NAME}
        SRC ${SIMEX_SOURCES} IPCBlockExample.cpp
        LINK_TO ${SIMEX_LIBS}
)
target_compile_definitions(${FUNCTION_NAME} PRIVATE S_FUNCTION_NAME=${FUNCTION_NAME})

set(FUNCTION_NAME IPCBlockTetheredExample)

find_package(Boost REQUIRED COMPONENTS filesystem)
matlab_add_mex(
        NAME ${FUNCTION_NAME}
        SRC ${SIMEX_SOURCES} IPCBlockTetheredExample.cpp
        LINK_TO ${SIMEX_LIBS} ${Boost_LIBRARIES}
)
target_compile_definitions(${FUNCTION_NAME} PRIVATE S_FUNCTION_NAME=${FUNCTION_NAME})