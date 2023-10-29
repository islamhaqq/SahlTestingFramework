cmake_minimum_required(VERSION 3.15)
project(TestingFramework)

# @TODO: Figure out a way to remove explicit inclusion of header files since they should be automatically included
file(GLOB SOURCES "*.cpp" "*.h")

add_library(TestingFrameworkLib ${SOURCES})
target_include_directories(TestingFrameworkLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    add_subdirectory(Tests)
endif()
