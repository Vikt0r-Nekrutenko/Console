#include "Window.hpp"
#include "Box.hpp"
#include <ctime>

Window::Window(const short x, const short y, const short width, const short height)
    : m_out(GetStdHandle(STD_OUTPUT_HANDLE)),
      m_in(GetStdHandle(STD_INPUT_HANDLE)),
      m_wnd(GetConsoleWindow())
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(m_out, &csbi);

    m_placeSize = { short(csbi.srWindow.Right + 1), short(csbi.srWindow.Bottom + 1)};

    RECT size;
    GetClientRect(m_wnd, &size);

    m_windowSize.X = short(size.right - size.left);
    m_windowSize.Y = short(size.bottom - size.top);

    MoveWindow(m_wnd,
               x == CENTER ? GetSystemMetrics(SM_CXSCREEN) / 2 - m_windowSize.X / 2 : x,
               y == CENTER ? GetSystemMetrics(SM_CYSCREEN) / 2 - m_windowSize.Y / 2 : y,
               m_windowSize.X,
               m_windowSize.Y,
               true);

    resize(width, height);
    setCursorParams();
}

void Window::resize(const short width, const short height)
{
    int minW = GetSystemMetrics(SM_CXMIN),
        minH = GetSystemMetrics(SM_CYMIN),
        maxW = GetLargestConsoleWindowSize(m_out).X * (m_windowSize.X / m_placeSize.X),
        maxH = GetLargestConsoleWindowSize(m_out).Y * (m_windowSize.Y / m_placeSize.Y);

    if ((width < minW) || (width > maxW) || (height < minH) || (height > maxH)) {
        MessageBoxA(m_wnd, "Your size isn't corrected!", "Alarm", MB_ICONWARNING);
        return;
    }

    resizePlace(short(width / (m_windowSize.X / m_placeSize.X)),
                short(height / (m_windowSize.Y / m_placeSize.Y)));
    m_windowSize = { width, height };
}

void Window::move(const short x, const short y)
{
    MoveWindow(m_wnd, x, y, m_windowSize.X, m_windowSize.Y, true);
}

void Window::setFontParams(const short width, const short height, const bool isBold, const wchar_t *fontFaceName)
{
    CONSOLE_FONT_INFOEX fontInfo;
    ZeroMemory(&fontInfo, sizeof(CONSOLE_FONT_INFOEX));
    fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);

    lstrcpyW(fontInfo.FaceName, fontFaceName);
    fontInfo.FontFamily = (wcscmp(fontFaceName, L"Terminal")) ? 54 : FF_MODERN;
    fontInfo.FontWeight = isBold ? FW_BOLD : FW_NORMAL;
    fontInfo.dwFontSize.X = width;
    fontInfo.dwFontSize.Y = height;
    SetCurrentConsoleFontEx(m_out, FALSE, &fontInfo);

    resizePlace(short(m_windowSize.X / width), short(m_windowSize.Y / height));
}

void Window::setCursorParams(const bool isVisible, const short size) const
{
    CONSOLE_CURSOR_INFO inf;
    inf.dwSize = DWORD(size);
    SetConsoleCursorInfo(m_out, &inf);
    inf.bVisible = isVisible;
    SetConsoleCursorInfo(m_out, &inf);
}

void Window::addNewControl(const std::initializer_list<Box *> controls)
{
    for (std::initializer_list<Box *>::iterator it = controls.begin(); it != controls.end(); it++) {
        m_controls.push_back(*it);
        (*it)->show();
    }
}

void Window::secondElased(const float fps)
{
    SetConsoleTitleA(std::to_string(fps).c_str());
}

void Window::setCursorPosition(const short x, const short y) const
{
    SetConsoleCursorPosition(m_out, { x, y });
}

void Window::resizePlace(const short width, const short height)
{
    if(width >= m_placeSize.X && height >= m_placeSize.Y){
        resizeBuffer(width, height);
        resizeRect(width, height);
    } else if(width <= m_placeSize.X && height <= m_placeSize.Y){
        resizeRect(width, height);
        resizeBuffer(width, height);
    } else if(width > m_placeSize.X && height < m_placeSize.Y) {
        resizeBuffer(width, m_placeSize.Y);
        resizeRect(width, height);
        resizeBuffer(width, height);
    } else if(width < m_placeSize.X && height > m_placeSize.Y){
        resizeBuffer(m_placeSize.X, height);
        resizeRect(width, height);
        resizeBuffer(width, height);
    } else {
        MessageBoxA(nullptr, "Uncorrected place size!", "Alarm", MB_ICONERROR);
    }

    m_placeSize.X = width;
    m_placeSize.Y = height;
}

void Window::resizeBuffer(const short width, const short height)
{
    if (!SetConsoleScreenBufferSize(m_out, { width, height })) {
        MessageBoxA(nullptr, "Place resizing error!", "Alarm", MB_ICONERROR);
    }
}

void Window::resizeRect(const short width, const short height)
{
    SMALL_RECT size = { 0, 0, short(width - 1), short(height - 1) };
    if (!SetConsoleWindowInfo(m_out, true, &size)) {
        MessageBoxA(nullptr, "Rect resizing error!", "Alarm", MB_ICONERROR);
    }
}

const HANDLE &Window::out() const
{
    return m_out;
}

void windowEventProc(Window *window)
{
    INPUT_RECORD ir[32];
    DWORD n;
    Box *activeControl = nullptr;
    float deltaTime = 0.f, elapsedTime = 0.f;
    size_t frames = 0ull;

    while (true)
    {
        if (elapsedTime > 1.f) {
            window->secondElased(elapsedTime / frames);
            elapsedTime = frames = 0;
        }

        time_t begin = clock();

        window->update(deltaTime);

        GetNumberOfConsoleInputEvents(window->m_in, &n);
        if (n > 0) {
            ReadConsoleInputA(window->m_in, ir, n, &n);

            for (DWORD i = 0; i < n; i++) {
                switch (ir[i].EventType)
                {
                case MOUSE_EVENT:
                {
                    short x = ir[i].Event.MouseEvent.dwMousePosition.X;
                    short y = ir[i].Event.MouseEvent.dwMousePosition.Y;

                    auto cursorInControl = [x, y](const Box *control) -> bool {
                        if (x > control->getFrame().Left && x < control->getFrame().Right && y > control->getFrame().Top && y < control->getFrame().Bottom) {
                            return true;
                        }
                        return false;
                    };

                    auto findControl = [&activeControl, cursorInControl, &window](void) -> bool {
                        for (Box *control : window->m_controls) {
                            if (cursorInControl(control)) {
                                if (activeControl != nullptr) {
                                    activeControl->deactivate();
                                }
                                activeControl = control;
                                activeControl->activate();
                                return true;
                            }
                        }
                        if (activeControl != nullptr) {
                            activeControl->deactivate();
                            activeControl = nullptr;
                        }
                        return false;
                    };

                    if (ir[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                        if (findControl()){
                            activeControl->mouseEventHandler(window, { MouseButton::LEFT, ir[i].Event.MouseEvent.dwMousePosition, !ir[i].Event.MouseEvent.dwEventFlags ? true : false });
                        } else {
                            window->mouseEventHandler(window, { MouseButton::LEFT, ir[i].Event.MouseEvent.dwMousePosition, !ir[i].Event.MouseEvent.dwEventFlags ? true : false });
                        }
                    } else if (ir[i].Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {
                        if (findControl()){
                            activeControl->mouseEventHandler(window, { MouseButton::RIGHT, ir[i].Event.MouseEvent.dwMousePosition, !ir[i].Event.MouseEvent.dwEventFlags ? true : false });
                        } else {
                            window->mouseEventHandler(window, { MouseButton::RIGHT, ir[i].Event.MouseEvent.dwMousePosition, !ir[i].Event.MouseEvent.dwEventFlags ? true : false });
                        }
                    }
                    if (ir[i].Event.MouseEvent.dwEventFlags == MOUSE_MOVED) {
                        if (activeControl != nullptr && cursorInControl(activeControl)){
                            activeControl->mouseEventHandler(window, { MouseButton::NONE, ir[i].Event.MouseEvent.dwMousePosition, !ir[i].Event.MouseEvent.dwEventFlags ? true : false });
                        } else {
                            window->mouseEventHandler(window, { MouseButton::NONE, ir[i].Event.MouseEvent.dwMousePosition, !ir[i].Event.MouseEvent.dwEventFlags ? true : false });
                        }
                    }
                    break;
                }
                case KEY_EVENT:
                {
                    if (ir[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) {
                        return;
                    } else if (ir[i].Event.KeyEvent.bKeyDown) {
                        if (activeControl == nullptr) {
                            window->keyEventHandler(window, { true, ir[i].Event.KeyEvent.uChar.AsciiChar, ir[i].Event.KeyEvent.wVirtualKeyCode });
                        } else {
                            activeControl->keyEventHandler(window, { true, ir[i].Event.KeyEvent.uChar.AsciiChar, ir[i].Event.KeyEvent.wVirtualKeyCode });
                        }
                    } else {
                        if (activeControl == nullptr) {
                            window->keyEventHandler(window, { false, ir[i].Event.KeyEvent.uChar.AsciiChar, ir[i].Event.KeyEvent.wVirtualKeyCode });
                        } else {
                            activeControl->keyEventHandler(window, { false, ir[i].Event.KeyEvent.uChar.AsciiChar, ir[i].Event.KeyEvent.wVirtualKeyCode });
                        }
                    }
                    break;
                }
                }
            }
        }

        deltaTime = (clock() - begin) / 1000.f;
        elapsedTime += deltaTime;
        ++frames;
    }
}
