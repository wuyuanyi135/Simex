include_directories(${SIMEX_INCLUDES})
add_executable(msg_test msg_test.cpp)
target_link_libraries(msg_test ${SIMEX_LIBS})