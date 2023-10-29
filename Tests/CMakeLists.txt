file(GLOB TEST_SOURCES "*.cpp")
add_executable(TestingFrameworkTests ${TEST_SOURCES})

target_link_libraries(TestingFrameworkTests TestingFrameworkLib)
