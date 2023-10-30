#include <csignal>
#include "TestingFramework.h"
#include "TestingTools.h"

#ifdef _WIN32

#include <Windows.h>

S_TEST(TestingTools, MouseClick)
{
    // Given an app opened to the top right-corner
    system("start wordpad.exe");
    Sleep(1000); // Wait to make sure Notepad is open
    HWND windowHandle = FindWindow(NULL, "Document - Wordpad");
    S_ASSERT_TRUE(IsWindow(windowHandle));
    MoveWindowToTopRight(windowHandle);

    // When I use testing tools to click at the top right
    RECT rect;
    GetWindowRect(windowHandle, &rect);
    TestingTools::MouseClick(rect.right - 20, rect.top + 20);
    Sleep(1000); // Wait to make sure Notepad is closed

    // Then it should be closed
    S_ASSERT_FALSE(IsWindow(windowHandle));
}

#endif // _WIN32

#ifdef __linux__

#include <chrono>
#include <thread>

S_TEST(TestingTools, ShouldVisiblyMoveMouse)
{
    // Given a call to MoveMouseTo(800, 400) with the destination being X: 2500, Y: 1000
    TestingTools::MoveMouseTo(800, 400);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // When I get the mouse position
    TestingTools::CursorProperties cursorProperties = TestingTools::GetCursorPosition();

    // Then the mouse should be at (800, 400)
    S_EXPECT_EQ(cursorProperties.x, 800);
    S_EXPECT_EQ(cursorProperties.y, 400);
}

S_TEST(TestingTools, MouseClick)
{
    // Given an app opened to the top right-corner
    system("firefox &");
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    Display* x11Display = XOpenDisplay(nullptr);
    Window root = DefaultRootWindow(x11Display);
    Window appWindow = TestingTools::FindWindowByName(x11Display, root, "Mozilla Firefox");
    if (appWindow == 0) S_FAIL();
    XRRCrtcInfo primaryMonitorCrtcInfo = TestingTools::GetPrimaryMonitor(x11Display);

    // When I use testing tools to click in the top right corner of my screen
    TestingTools::MouseClick(primaryMonitorCrtcInfo.x + primaryMonitorCrtcInfo.width - 50, primaryMonitorCrtcInfo.y + 50);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Then the app window should be closed
    S_EXPECT_EQ(TestingTools::FindWindowByName(x11Display, root, "Mozilla Firefox"), 0);
}

#endif // __linux__
