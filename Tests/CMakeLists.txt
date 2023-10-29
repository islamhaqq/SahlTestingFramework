file(GLOB TEST_SOURCES "*.cpp")
list(REMOVE_ITEM TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

add_executable(TestingFrameworkTests ${TEST_SOURCES} main.cpp)

target_link_libraries(TestingFrameworkTests TestingFrameworkLib)
