project(TestingFramework)

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/*.cpp")

add_library(TestingFramework ${SOURCES})

add_subdirectory(Tests)