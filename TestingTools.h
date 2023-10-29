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

#include <linux/uinput.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <cstdint>

void createVirtualMouse(int &fd) {
    struct uinput_setup usetup;

    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

    ioctl(fd, UI_SET_EVBIT, EV_KEY);
    ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);

    ioctl(fd, UI_SET_EVBIT, EV_REL);
    ioctl(fd, UI_SET_RELBIT, REL_X);
    ioctl(fd, UI_SET_RELBIT, REL_Y);

    memset(&usetup, 0, sizeof(usetup));
    usetup.id.bustype = BUS_USB;
    usetup.id.vendor = 0x1234;
    usetup.id.product = 0x5678;
    strcpy(usetup.name, "Example Virtual Mouse");

    ioctl(fd, UI_DEV_SETUP, &usetup);
    ioctl(fd, UI_DEV_CREATE);
}

void sendEvent(int fd, uint16_t type, uint16_t code, int32_t value) {
    struct input_event ie;

    ie.type = type;
    ie.code = code;
    ie.value = value;
    ie.time.tv_sec = 0;
    ie.time.tv_usec = 0;

    write(fd, &ie, sizeof(ie));
}


#endif // __linux__