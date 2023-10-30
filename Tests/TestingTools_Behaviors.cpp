#include <csignal>
#include "TestingFramework.h"
#include "TestingTools.h"

#ifdef _WIN32

#include <Windows.h>

void MoveWindowToTopRight(HWND hWnd)
{
    RECT windowRectangle;
    GetWindowRect(hWnd, &windowRectangle);

    int windowWidth = windowRectangle.right - windowRectangle.left;
    int windowHeight = windowRectangle.bottom - windowRectangle.top;

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);

    MoveWindow(hWnd, screenWidth - windowWidth, 0, windowWidth, windowHeight, TRUE);
}

S_TEST(TestingTools, MouseClick)
{
    // Given an app opened to the top right-corner
    system("start notepad.exe");
    Sleep(1000); // Wait to make sure Notepad is open
    HWND windowHandle = FindWindow(NULL, "Untitled - Notepad");
    S_ASSERT_TRUE(IsWindow(windowHandle));
    MoveWindowToTopRight(windowHandle);

    // When I use testing tools to click at the top right
    RECT rect;
    GetWindowRect(windowHandle, &rect);
    TestingTools::MouseClick(TestingTools::MouseButtons::Left, rect.right - 20, rect.top + 20);
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
    // Given a call to MoveMouseTo(2500, 1000) with the destination being X: 2500, Y: 1000
    MoveMouseTo(2500, 1000);

    // When I get the mouse position
    CursorProperties cursorProperties = GetCursorPosition();

    // Then the mouse should be at (2500, 1000)
    S_EXPECT_EQ(cursorProperties.x, 2500);
    S_EXPECT_EQ(cursorProperties.y, 1000);
}

S_TEST(TestingTools, MouseClick)
{
    // Given an app opened to the top right-corner
    system("gedit &");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    Display* x11Display = XOpenDisplay(nullptr);
    Window root = DefaultRootWindow(x11Display);
    Window appWindow = FindWindowByName(x11Display, root, "Untitled Document 1 - gedit");
    if (appWindow == 0) S_FAIL();
    XRRCrtcInfo primaryMonitorCrtcInfo = GetPrimaryMonitor(x11Display);
    MoveWindowToTopRightCorner(x11Display, appWindow, primaryMonitorCrtcInfo);

    // When I use testing tools to click in the top right corner of my screen
    XWarpPointer(x11Display, None, root, 0, 0, 0, 0, primaryMonitorCrtcInfo.x + primaryMonitorCrtcInfo.width - 50, primaryMonitorCrtcInfo.y + 50);
    MouseClick(x11Display);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Then the app window should be closed
    S_EXPECT_EQ(FindWindowByName(x11Display, root, "Untitled Document 1 - gedit"), 0);
}

#endif // __linux__
