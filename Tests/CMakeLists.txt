file(GLOB TEST_SOURCES "*.cpp")

add_executable(TestingFrameworkTests ${TEST_SOURCES})
target_link_libraries(TestingFrameworkTests TestingFrameworkLib)

# if linux require x11
if (UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
    include_directories(${X11_INCLUDE_DIR})
    target_link_libraries(TestingFrameworkTests ${X11_LIBRARIES})
endif()
