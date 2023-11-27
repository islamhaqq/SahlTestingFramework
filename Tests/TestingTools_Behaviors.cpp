#include "TestingFramework.h"
#include "TestingTools.h"

#include <chrono>
#include <thread>

S_TEST(TestingTools, MouseClickShouldVisiblyMoveMouse)
{
    TestingTools::MouseClick(800, 400);
    TestingTools::CursorProperties cursorProperties = TestingTools::GetCursorPosition();
    S_EXPECT_EQ(cursorProperties.x, 800);
    S_EXPECT_EQ(cursorProperties.y, 400);

    TestingTools::MouseClick(600, 500);
    cursorProperties = TestingTools::GetCursorPosition();
    S_EXPECT_EQ(cursorProperties.x, 600);
    S_EXPECT_EQ(cursorProperties.y, 500);
}

#ifdef _WIN32

#include <Windows.h>

S_TEST(TestingTools, MouseClick)
{
    // Given an app opened
    system("start wordpad.exe");
    std::this_thread::sleep_for(std::chrono::milliseconds(250)); // Wait to make sure Notepad is open
    HWND windowHandle = FindWindow(NULL, "Document - Wordpad");
    S_ASSERT_TRUE(IsWindow(windowHandle));

    // When I use MouseClick to click in the window's "X" (close) button
    RECT rect;
    GetWindowRect(windowHandle, &rect);
    TestingTools::MouseClick(rect.right - 20, rect.top + 20);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));

    // Then it should be closed
    S_ASSERT_FALSE(IsWindow(windowHandle));
}

#endif // _WIN32

#ifdef __linux__

S_TEST(TestingTools, MouseClickCanCloseWindow)
{
    // Given an app opened to the top right-corner
    system("firefox &");
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    Display* x11Display = XOpenDisplay(nullptr);
    Window root = DefaultRootWindow(x11Display);
    Window appWindow = TestingTools::FindWindowByName(x11Display, root, "Mozilla Firefox");
    if (appWindow == 0) S_FAIL();
    XRRCrtcInfo primaryMonitorCrtcInfo = TestingTools::GetPrimaryMonitor(x11Display);
    TestingTools::MoveWindowToTopRight(x11Display, appWindow, primaryMonitorCrtcInfo);

    // When I use testing tools to click in the top right corner of my screen
    TestingTools::MouseClick(primaryMonitorCrtcInfo.x + primaryMonitorCrtcInfo.width - 50, primaryMonitorCrtcInfo.y + 50);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Then the app window should be closed
    S_EXPECT_EQ(TestingTools::FindWindowByName(x11Display, root, "Mozilla Firefox"), 0);
}

#endif // __linux__
