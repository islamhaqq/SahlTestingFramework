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

#include <stdio.h>
#include <X11/Xlib.h>

S_TEST(TestingTools, ShouldVisiblyMoveMouse)
{
    // Given the original mouse position
    Display* x11Display = XOpenDisplay(NULL);
    Window rootWindow = DefaultRootWindow(x11Display);
    Window rootWindowUnderMouse;
    Window childWindowUnderMouse;
    unsigned int bitMaskForModifierKeys;

    int originalMouseX;
    int originalMouseY;
    int mouseXUnderChildWindow;
    int mouseYUnderChildWindow;
    XQueryPointer(
            x11Display,
            rootWindow,
            &rootWindowUnderMouse,
            &childWindowUnderMouse,
            &originalMouseX,
            &originalMouseY,
            &mouseXUnderChildWindow,
            &mouseYUnderChildWindow,
            &bitMaskForModifierKeys
    );


    // When I call MoveMouseTo()
    MoveMouseTo();

    // Then the mouse should not be in its original position
    int newMouseX;
    int newMouseY;
    int newMouseXUnderChildWindow;
    int newMouseYUnderChildWindow;
    XQueryPointer(
            x11Display,
            rootWindow,
            &rootWindowUnderMouse,
            &childWindowUnderMouse,
            &newMouseX,
            &newMouseY,
            &newMouseXUnderChildWindow,
            &newMouseYUnderChildWindow,
            &bitMaskForModifierKeys
    );
    S_EXPECT_NE(originalMouseX, newMouseX);
    XCloseDisplay(x11Display);
}
//
//S_TEST(TestingTools, MouseClick)
//{
//
//}

#endif // __linux__
