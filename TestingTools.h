#pragma once

namespace TestingTools
{
    struct CursorProperties
    {
        int x;
        int y;
    };

    CursorProperties GetCursorPosition();
    void MouseClick(int x, int y);
}

#ifdef __linux__

#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

namespace TestingTools
{
    Window FindWindowByName(Display *display, Window root, const char *name);
    XRRCrtcInfo GetPrimaryMonitor(Display *display);
    void MoveWindowToTopRight(Display *display, Window window, XRRCrtcInfo targetMonitor);
}

#endif // __linux__
