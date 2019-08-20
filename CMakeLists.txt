cmake_minimum_required(VERSION 3.14)
project(Simex)

find_package(Matlab REQUIRED)
set(Simulink_DIR ${Matlab_ROOT_DIR}/simulink)

include_directories(${Matlab_INCLUDE_DIRS})
include_directories(${Simulink_DIR}/include)
include_directories(.)

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
add_compile_definitions(S_FUNCTION_LEVEL=2)

# options

file(GLOB SIMULINK_IMPL_SOURCES simulink_impl/*.c simulink_impl/*.cpp)
file(GLOB CPP_API_SOURCES cpp_api/*.c cpp_api/*.cpp)
set(
        SIMEX_INCLUDES
        ${CMAKE_CURRENT_LIST_DIR}/cpp_api
        ${CMAKE_CURRENT_LIST_DIR}/simulink_impl
        ${CMAKE_CURRENT_LIST_DIR}/cpp_api/ipc/msgpack-c
)
set (
        SIMEX_SOURCES
        ${SIMULINK_IMPL_SOURCES}
        ${CPP_API_SOURCES}
)

if (${ENABLE_TEST})
    file(GLOB TEST_DIRS LIST_DIRECTORIES true test/*)

    foreach (TEST_DIR in ${TEST_DIRS})
        if (IS_DIRECTORY ${TEST_DIR})
            message(Adding test: ${TEST_DIR})
            add_subdirectory(${TEST_DIR})

        endif ()
    endforeach ()
endif ()
