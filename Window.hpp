#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <vector>
#include "BasicTemplate.hpp"

#define CLOSE_EVENT 0xFF

namespace
{
    const short CENTER { 0xfff };
    const short DEF_WND_WIDTH { 640 };
    const short DEF_WND_HEIGHT{ 480 };
    const wchar_t *Lucida_Console { L"Lucida Console" };
    const wchar_t *Consolas       { L"Consolas" };
    const wchar_t *Terminal       { L"Terminal" };
}

class Box;

class Window : public BasicTemplate
{
    friend void windowEventProc(Window *window);
public:
    Window(const short x = CENTER, const short y = CENTER, const short width = DEF_WND_WIDTH, const short height = DEF_WND_HEIGHT);
    void resize(const short width, const short height) override;
    void move(const short x, const short y) override;
    void setFontParams(const short width = 8, const short height = 12, const bool isBold = false, const wchar_t *fontFaceName = Terminal);
    void setCursorParams(const bool isVisible = false, const short size = 1) const;
    void setCursorPosition(const short x, const short y) const;
    const HANDLE &out() const;

protected:
    std::vector <Box *> m_controls;
    COORD m_windowSize, m_placeSize;
    HANDLE m_out, m_in;
    HWND m_wnd;

private:
    void resizePlace(const short width, const short height);
    void resizeBuffer(const short width, const short height);
    void resizeRect(const short width, const short height);
};

void windowEventProc(Window *window);

#endif // WINDOW_HPP
