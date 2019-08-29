set(ENABLE_THIS_TEST 0)

if (${ENABLE_THIS_TEST})
    set(EXAMPLE_NAME SubprocessIPCExample)
    add_compile_definitions(S_FUNCTION_NAME=${EXAMPLE_NAME})

    set(CMAKE_CXX_STANDARD 14)
    set(TEST_SOURCES SubprocessIPCExample.cpp)
    file(GLOB UI_SOURCES ui/*.cpp)

    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    set(CMAKE_INSTALL_RPATH "/usr/lib/x86_64-linux-gnu")


    add_compile_definitions(DEBUG)
    #add_compile_definitions(DEBUG_VERBOSITY_LIFECYCLE)
    #add_compile_definitions(DEBUG_SIMULINK_CALLBACK)

    set(CMAKE_BUILD_TYPE Debug)
    # Instruct CMake to run moc automatically when needed
    set(CMAKE_AUTOMOC ON)

    find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
    find_package(Boost COMPONENTS system filesystem)

    matlab_add_mex(
            NAME ${EXAMPLE_NAME}
            SRC ${SIMULINK_IMPL_SOURCES} ${CPP_API_SOURCES} ${TEST_SOURCES}
            LINK_TO ${Matlab_MEX_LIBRARY} ${Boost_LIBRARIES} pthread rt
    )


    set(CMAKE_CXX_FLAGS -fPIC)

    include_directories(${Qt5Widgets_INCLUDE_DIRS})


    add_executable(SubprocessIPCDisplay ${UI_SOURCES})
    target_link_libraries(SubprocessIPCDisplay Qt5::Widgets ${Boost_LIBRARIES} pthread rt)
    add_dependencies(${EXAMPLE_NAME} SubprocessIPCDisplay)

    add_executable(SubprocessIPCSimulate mock/simulate.cpp)
    target_link_libraries(SubprocessIPCSimulate Qt5::Widgets ${Boost_LIBRARIES} pthread rt)
    add_dependencies(SubprocessIPCSimulate SubprocessIPCDisplay)
endif ()