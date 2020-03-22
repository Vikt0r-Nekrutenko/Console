#include "Event.hpp"
#include "Window.hpp"

void Event::operator =(const TMouseEventHandler handler)
{
    m_mouseHandler = handler;
}

void Event::operator =(const TKeyEventHandler handler)
{
    m_keyHandler = handler;
}

void Event::call(Window *sender, const MouseRecord mouseRecord)
{
    if (m_mouseHandler != nullptr) {
        (sender->*m_mouseHandler)(mouseRecord);
    }
}

void Event::call(Window *sender, const KeyRecord keyRecord)
{
    if (m_keyHandler != nullptr) {
        (sender->*m_keyHandler)(keyRecord);
    }
}
