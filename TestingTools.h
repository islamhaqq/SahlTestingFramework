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