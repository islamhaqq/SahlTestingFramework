cmake_minimum_required(VERSION 3.15)
project(TestingFramework)

# @TODO: Figure out a way to remove explicit inclusion of header files since they should be automatically included
file(GLOB SOURCES "*.cpp" "*.h")

add_library(TestingFrameworkLib ${SOURCES})
target_include_directories(TestingFrameworkLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
    include_directories(${X11_INCLUDE_DIR})
    target_link_libraries(TestingFrameworkLib ${X11_LIBRARIES})

    if (X11_Xrandr_FOUND)
        target_link_libraries(TestingFrameworkLib ${X11_Xrandr_LIB})
    else()
        message(WARNING "Xrandr not found!")
    endif()

    if (X11_Xtst_FOUND)
        target_link_libraries(TestingFrameworkLib ${X11_Xtst_LIB})
    else()
        message(WARNING "XTest not found!")
    endif()
endif()

# Add test suites for the testing framework.
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    add_subdirectory(Tests)
    add_subdirectory(NewTests)
endif()
