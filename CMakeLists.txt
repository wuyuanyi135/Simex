cmake_minimum_required(VERSION 3.14)
project(Simex)

find_package(Matlab REQUIRED)
# TODO: optionally build ipc
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS system unit_test_framework)


set(Simulink_DIR ${Matlab_ROOT_DIR}/simulink)

include_directories(${Matlab_INCLUDE_DIRS})
include_directories(${Simulink_DIR}/include)
include_directories(.)

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
add_compile_definitions(S_FUNCTION_LEVEL=2)

# options

file(GLOB SIMULINK_IMPL_SOURCES simulink_impl/*.c simulink_impl/*.cpp)
file(GLOB CPP_API_SOURCES cpp_api/*.c cpp_api/*.cpp cpp_api/ipc/*.cpp)
set(
        SIMEX_INCLUDES
        ${CMAKE_CURRENT_LIST_DIR}/cpp_api
        ${CMAKE_CURRENT_LIST_DIR}/simulink_impl
        ${CMAKE_CURRENT_LIST_DIR}/cpp_api/ipc/msgpack-c/include
)
set (
        SIMEX_SOURCES
        ${SIMULINK_IMPL_SOURCES}
        ${CPP_API_SOURCES}
)
set (
        SIMEX_LIBS
        ${Boost_LIBRARIES}
)
set (
        SIMEX_TESTS
        ${CMAKE_CURRENT_LIST_DIR}/cpp_api/ipc/test

)

if (${ENABLE_TEST})
    file(GLOB TEST_DIRS LIST_DIRECTORIES true test/*)

    foreach (TEST_DIR ${TEST_DIRS})
        if (IS_DIRECTORY ${TEST_DIR})
            message(Adding test: ${TEST_DIR})
            add_subdirectory(${TEST_DIR})
        endif ()
    endforeach ()
    foreach (UNITTEST_DIR ${SIMEX_TESTS})
#        message(${UNITTEST_DIR})
        add_subdirectory(${UNITTEST_DIR} )
    endforeach ()
endif ()
