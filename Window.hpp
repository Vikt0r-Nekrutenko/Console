#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <windows.h>

#define CENTER 0xfff
#define CLOSE_EVENT 0xFF

enum class MouseButton {
    LEFT = 0x0001,
    RIGHT = 0x0002,
};

struct Key {
    char symbol;
    unsigned short keyCode;
};

class Window
{
    friend void windowEventProc(Window *window);
public:
    Window(const short x = CENTER, const short y = CENTER, const short width = 640, const short height = 480);
    virtual ~Window() = default;
    void resize(const short width, const short height);

protected:
    virtual void onMouseMove(const short x, const short y);
    virtual void onMouseButtonClick(MouseButton button);
    virtual void onKeyPressed(Key key);
    virtual void onKeyReleased(Key key);

private:
    void resizePlace(const short width, const short height);
    void resizeBuffer(const short width, const short height);
    void resizeRect(const short width, const short height);

    COORD m_windowSize, m_placeSize;
    HANDLE m_out, m_in;
    HWND m_wnd;
};

void windowEventProc(Window *window);

#endif // WINDOW_HPP
