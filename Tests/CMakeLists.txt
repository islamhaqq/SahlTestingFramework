project(TestingFrameworkTests)

file(GLOB_RECURSE TEST_SOURCES "*.cpp")

add_executable(${PROJECT_NAME} ${TEST_SOURCES})

target_link_libraries(${PROJECT_NAME} TestingFramework)
