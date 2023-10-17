project(TestingFramework)

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/*.cpp" "${PROJECT_SOURCE_DIR}/*.h")

add_library(TestingFramework ${SOURCES})

target_include_directories(EngineLib PUBLIC ${CMAKE_SOURCE_DIR}/libs/TestingFramework)

add_subdirectory(Tests)
