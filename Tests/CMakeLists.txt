file(GLOB TEST_SOURCES "*.cpp")

add_executable(TestingFrameworkTests ${TEST_SOURCES})
target_link_libraries(TestingFrameworkTests TestingFrameworkLib)

if (UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
    include_directories(${X11_INCLUDE_DIR})
    target_link_libraries(TestingFrameworkTests ${X11_LIBRARIES})

    if (X11_Xrandr_FOUND)
        target_link_libraries(TestingFrameworkTests ${X11_Xrandr_LIB})
    else()
        message(WARNING "Xrandr not found!")
    endif()

    if (X11_Xtst_FOUND)
        target_link_libraries(TestingFrameworkTests ${X11_Xtst_LIB})
    else()
        message(WARNING "XTest not found!")
    endif()
endif()
