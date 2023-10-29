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

void MoveMouseTo()
{
    Display* x11Display = XOpenDisplay(NULL);
    Window rootWindow = DefaultRootWindow(x11Display);
    XWarpPointer(x11Display, None, None, 0, 0, 0, 0, 10, 0);

    // Flush the output buffer and ensure move is applied.
    XFlush(x11Display);
    XCloseDisplay(x11Display);
}

#endif // __linux__
