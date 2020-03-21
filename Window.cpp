#include "Window.hpp"
#include <iostream>

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

void Window::onMouseButtonClick(MouseButton button) {

}

void Window::onKeyPressed(Key key) {

}

void Window::onKeyReleased(Key key) {

}

void Window::onMouseMove(const short x, const short y) {

}

void Window::resizePlace(const short width, const short height)
{
    if(width >= m_placeSize.X && height >= m_placeSize.Y){ /// �u���|�y �r�r���t�y�}���u ���p�x�}�u���� �q���|�����u �~���~�u���~�y��: �y�x�}�u�~���u�} ���p�x�}�u�� �q�����u���p, �y�x�}�u�~���u�} ���p�x�}�u�� ���{�~�p
        resizeBuffer(width, height);
        resizeRect(width, height);
    } else if(width <= m_placeSize.X && height <= m_placeSize.Y){ /// �u���|�y �r�r���t�y�}���u ���p�x�}�u���� �}�u�~�����u �~���~�u���~�y��: �y�x�}�u�~���u�} ���p�x�}�u�� ���{�~�p, �x�p���u�} �q�����u���p
        resizeRect(width, height);
        resizeBuffer(width, height);
    } else if(width > m_placeSize.X && height < m_placeSize.Y){ /// �u���|�y ���y���y�~�p �q���|�����u, �p �r���������p �}�u�~�����u, ������������ ���p�x�}�u�� �y�x�}�u�~�y���� �~�u �����|�����y�������� �x�p �����u�� �����p�r�y�|�p "�q�����u�� �~�y�{���s�t�p �~�u �}�u�~�����u ���{�~�p"
        resizeBuffer(width, m_placeSize.Y); /// ���~�p���p�|�p �y�x�}�u�~���u�} ���y���y�~�� �q�����u���p(�r���������� �������p�r�|���u�})
        resizeRect(width, height); /// ���u���u���� ���p�������s�y�r�p�u�} ���{�~��(�q�����u�� �B�R�E�C�D�@ �q���|�����u ���{�~�p)
        resizeBuffer(width, height); /// �����t�����s�y�r�p�u�} �q�����u��
    } else if(width < m_placeSize.X && height > m_placeSize.Y){ /// �u���|�y ���y���y�~�p �}�u�~�����u, �p �r���������p �q���|�����u ���y�����p���y�� ���p�w�u
        resizeBuffer(m_placeSize.X, height); /// �}�u�~���u�} �r���������� �������p�r�|���� ���y���y�~��
        resizeRect(width, height); /// �q�����u�� �q���|�����u ���{�~�p
        resizeBuffer(width, height); /// �����t�����s�y�r�p�u�} �q�����u��
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

void windowEventProc(Window *window)
{
    INPUT_RECORD ir;
    DWORD n;

    while (ir.Event.KeyEvent.wVirtualKeyCode != VK_ESCAPE)
    {
        WaitForSingleObject(window->m_in, INFINITE);
        ReadConsoleInputA(window->m_in, &ir, 1, &n);

        switch (ir.EventType)
        {
        case MOUSE_EVENT:
            if (ir.Event.MouseEvent.dwEventFlags == MOUSE_MOVED) {
                window->onMouseMove(ir.Event.MouseEvent.dwMousePosition.X, ir.Event.MouseEvent.dwMousePosition.Y);
            } else if (ir.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                window->onMouseButtonClick(MouseButton::LEFT);
            } else if (ir.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {
                window->onMouseButtonClick(MouseButton::RIGHT); break;
            }
            break;
        case KEY_EVENT:
            if (ir.Event.KeyEvent.bKeyDown) {
                window->onKeyPressed({ ir.Event.KeyEvent.uChar.AsciiChar, ir.Event.KeyEvent.wVirtualKeyCode });
            } else {
                window->onKeyReleased({ ir.Event.KeyEvent.uChar.AsciiChar, ir.Event.KeyEvent.wVirtualKeyCode });
            }
            break;
        case CLOSE_EVENT:
            return;
        }
    }
}
