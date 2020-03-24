#include "InputBox.hpp"
#include "Window.hpp"

InputBox::InputBox(const Window *parent, const short x, const short y, const short width, const short height)
    : Box(parent, x, y, width, height),
      m_lenVisibleText(size_t((width - 2) * (height - 2)) - 1),
      m_lastSymbol(m_lenVisibleText)
{

}

void InputBox::show()
{
    showText();
    Box::show();
}

void InputBox::activate()
{
    Box::activate();
    moveCursor();
    m_parent->setCursorParams(true);
}

void InputBox::deactivate()
{
    Box::deactivate();
    m_parent->setCursorParams(false);
}

void InputBox::resize(const short width, const short height)
{
    Box::resize(width, height);
    m_lenVisibleText = size_t((m_size.X - 2) * (m_size.Y - 2) - 1);
    m_lastSymbol = m_text.length();
    m_innerCursor = (m_text.length() > m_lenVisibleText) ? m_lenVisibleText : m_lastSymbol;
    m_firstSymbol = (short(m_lastSymbol - m_lenVisibleText) > 0) ? m_lastSymbol - m_lenVisibleText : 0ull;

    show();
}

void InputBox::setText(const std::string &text)
{
    m_text = text;
    m_symbolInText = m_lastSymbol = m_text.length();
    m_innerCursor = (m_text.length() > m_lenVisibleText) ? m_lenVisibleText : m_lastSymbol;
    m_firstSymbol = (short(m_lastSymbol - m_lenVisibleText) > 0) ? m_lastSymbol - m_lenVisibleText : 0ull;

    show();
}

void InputBox::keyEventHandler(Window *sender, const KeyRecord keyRecord)
{
    /*{
        m_parent->setCursorPosition(50, 0);
        printf("                        ");
        m_parent->setCursorPosition(50, 0);
        printf("first: %d", m_firstSymbol);
        m_parent->setCursorPosition(50, 1);
        printf("                        ");
        m_parent->setCursorPosition(50, 1);
        printf("last: %d", m_lastSymbol);
        m_parent->setCursorPosition(50, 2);
        printf("                        ");
        m_parent->setCursorPosition(50, 2);
        printf("in txt: %d", m_symbolInText);
        m_parent->setCursorPosition(50, 3);
        printf("                        ");
        m_parent->setCursorPosition(50, 3);
        printf("txt: %d", m_text.length());
    }*/

    if (keyRecord.isPressed && !isControlKeyPressed(keyRecord.keyCode)) {
        if ((keyRecord.symbol >= '0' && keyRecord.symbol <= 'z') || keyRecord.keyCode == VK_SPACE) {
            m_text.insert(m_symbolInText, 1, keyRecord.symbol);

            if (m_lastSymbol <= m_text.length()) {
                m_lastSymbol++;
            }
            if (m_text.length() > m_lenVisibleText) {
                m_firstSymbol++;
            } else {
                scrollRight();
            }
            show();
            moveCursor();
        }
    }
    keyEvent.call(sender, keyRecord);

    /*{
        m_parent->setCursorPosition(50, 0);
        printf("                        ");
        m_parent->setCursorPosition(50, 0);
        printf("first: %d", m_firstSymbol);
        m_parent->setCursorPosition(50, 1);
        printf("                        ");
        m_parent->setCursorPosition(50, 1);
        printf("last: %d", m_lastSymbol);
        m_parent->setCursorPosition(50, 2);
        printf("                        ");
        m_parent->setCursorPosition(50, 2);
        printf("in txt: %d", m_symbolInText);
        m_parent->setCursorPosition(50, 3);
        printf("                        ");
        m_parent->setCursorPosition(50, 3);
        printf("txt: %d", m_text.length());
        m_parent->setCursorPosition(50, 4);
        printf("                        ");
        m_parent->setCursorPosition(50, 4);
        printf("inner: %d", m_innerCursor);
        moveCursor();
    }*/
}

CHAR_INFO &InputBox::get(const size_t index)
{
    size_t x = size_t(index % size_t(m_size.X - 2));
    size_t y = size_t(index / size_t(m_size.X - 2));

    if (x >= size_t(m_size.X - 2)) x = size_t(m_size.X - 3);
    if (y >= size_t(m_size.Y - 2)) y = size_t(m_size.Y - 3);

    return m_rect[size_t(m_size.X) * (y + 1) + (x + 1)];
}

const std::string &InputBox::getText() const
{
    return m_text;
}

bool InputBox::isControlKeyPressed(unsigned short key)
{
    switch (key) {
    case VK_LEFT:
    {
        scrollLeft();
        show();
        moveCursor();
        return true;
    }
    case VK_RIGHT:
    {
        scrollRight();
        show();
        moveCursor();
        return true;
    }
    case VK_BACK:
    {
        if (!m_text.empty() && m_symbolInText) {
            m_text.erase(m_symbolInText - 1, 1);
        }
        scrollLeft();
        show();
        moveCursor();
        return true;
    }
    case VK_DELETE:
    {
        if (!m_text.empty() && m_symbolInText < m_text.length()) {
            m_text.erase(m_symbolInText + 1, 1);
        }
        show();
        moveCursor();
        return true;
    }
    default:
        return false;
    }
}

void InputBox::showText()
{
    for (size_t i = 0; i <= m_lenVisibleText; i++) {
        get(i).Char.AsciiChar = ' ';
    }
    for(size_t i = m_firstSymbol, j = 0; i < m_text.length() && i < m_lastSymbol; i++, j++) {
        get(j).Char.AsciiChar = m_text.at(i);
    }
}

void InputBox::moveCursor()
{
    short x = short(m_innerCursor / size_t(m_size.X - 2));
    short y = short(m_innerCursor % size_t(m_size.X - 2));
    m_parent->setCursorPosition(m_frame.Left + y + 1, m_frame.Top + x + 1);
}

void InputBox::scrollRight()
{
    if (m_symbolInText < m_text.length()) {
        m_symbolInText++;
    }
    if (m_innerCursor < m_lenVisibleText && m_innerCursor < m_text.length() - m_firstSymbol) {
        m_innerCursor++;
    } else {
        if (m_lastSymbol - m_firstSymbol == m_lenVisibleText + 1) m_firstSymbol++;
        if (m_lastSymbol < m_text.length()) m_lastSymbol++;
    }
}

void InputBox::scrollLeft()
{
    if (m_symbolInText > 0) {
        m_symbolInText--;
    }
    if(m_innerCursor > 0) {
        m_innerCursor--;
    } else {
        if (m_firstSymbol > 0) m_firstSymbol--;
        if (m_lastSymbol  > 0 && m_lastSymbol > m_lenVisibleText) m_lastSymbol--;
    }
}
