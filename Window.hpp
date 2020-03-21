#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <windows.h>

#define CENTER 0xfff

class Window
{
public:
    Window(const short x = CENTER, const short y = CENTER, const short width = 640, const short height = 480);
    void resize(const short width, const short height);
private:
    void resizePlace(const short width, const short height);
    void resizeBuffer(const short width, const short height);
    void resizeRect(const short width, const short height);

    COORD m_windowSize, m_placeSize;
    HANDLE m_out, m_in;
    HWND m_wnd;
};

#endif // WINDOW_HPP
