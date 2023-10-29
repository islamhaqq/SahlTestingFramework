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
