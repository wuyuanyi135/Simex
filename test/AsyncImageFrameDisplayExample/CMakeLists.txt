set(ENABLE_THIS_TEST 0)

if (${ENABLE_THIS_TEST})
    add_compile_definitions(S_FUNCTION_NAME=AsyncImageFrameDisplay)
    file(GLOB TEST_SOURCES *.cpp *.c)
    add_compile_definitions(DEBUG)
    #add_compile_definitions(DEBUG_VERBOSITY_LIFECYCLE)
    #add_compile_definitions(DEBUG_SIMULINK_CALLBACK)

    set(CMAKE_BUILD_TYPE Debug)

    find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
    include_directories(${Qt5Widgets_INCLUDE_DIRS})
    include_directories(${SIMEX_INCLUDES})

    matlab_add_mex(
            NAME AsyncImageFrameDisplay
            SRC ${SIMEX_SOURCES} ${TEST_SOURCES}
            LINK_TO ${Matlab_MEX_LIBRARY} ${SIMEX_LIBS}
    )

    add_executable(DisplayWindow Display.cpp)
    target_link_libraries(DisplayWindow Qt5::Core Qt5::Widgets)
endif ()
