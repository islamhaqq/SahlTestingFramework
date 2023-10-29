#ifdef _WIN32


#include <Windows.h>

namespace TestingTools {
    enum class MouseButtons {
        Left
    };

    void MouseClick(MouseButtons button, int x, int y) {
        SetCursorPos(x, y);

        mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
    }
}

#endif // _WIN32

#ifdef __linux__

#include <X11/Xlib.h>

struct CursorProperties
{
    int x;
    int y;
};

CursorProperties GetCursorPosition()
{
    Display* x11Display = XOpenDisplay(nullptr);
    Window rootWindow = DefaultRootWindow(x11Display);

    Window rootWindowUnderMouse;
    Window childWindowUnderMouse;
    int newMouseXUnderChildWindow;
    int newMouseYUnderChildWindow;

    unsigned int bitMaskForModifierKeys;

    CursorProperties cursorProperties{};

    XQueryPointer(
            x11Display,
            rootWindow,
            &rootWindowUnderMouse,
            &childWindowUnderMouse,
            &cursorProperties.x,
            &cursorProperties.y,
            &newMouseXUnderChildWindow,
            &newMouseYUnderChildWindow,
            &bitMaskForModifierKeys
    );

    XCloseDisplay(x11Display);

    return cursorProperties;
}

void MoveMouseTo(int x, int y)
{
    Display* x11Display = XOpenDisplay(nullptr);
    Window rootWindow = DefaultRootWindow(x11Display);
    XWarpPointer(x11Display, None, rootWindow, 0, 0, 0, 0, x, y);

    // Flush the output buffer and ensure move is applied.
    XFlush(x11Display);
    XCloseDisplay(x11Display);
}

#endif // __linux__
