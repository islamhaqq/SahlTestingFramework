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

S_TEST(TestingTools, MouseClick)
{
    int fd;
    createVirtualMouse(fd);

    sleep(1);

    // Move mouse 100 units on X axis
    sendEvent(fd, EV_REL, REL_X, 100);
    sendEvent(fd, EV_SYN, SYN_REPORT, 0);

    // Click left mouse button
    sendEvent(fd, EV_KEY, BTN_LEFT, 1);
    sendEvent(fd, EV_SYN, SYN_REPORT, 0);
    sendEvent(fd, EV_KEY, BTN_LEFT, 0);
    sendEvent(fd, EV_SYN, SYN_REPORT, 0);

    ioctl(fd, UI_DEV_DESTROY);
    close(fd);
}

#endif // __linux__