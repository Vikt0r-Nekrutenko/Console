#include "OutputBox.hpp"

OutputBox::OutputBox(const Window *parent, const short x, const short y, const short width, const short height)
    : InputBox(parent, x, y, width, height)
{

}

void OutputBox::keyEventHandler(Window *sender, const KeyRecord keyRecord)
{
    if (keyRecord.isPressed) {
        isControlKeyPressed(keyRecord.keyCode);
    }
    keyEvent.call(sender, keyRecord);
}

bool OutputBox::isControlKeyPressed(unsigned short key)
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
    default:
        return false;
    }
}
