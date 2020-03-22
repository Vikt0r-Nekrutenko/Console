#include "BasicTemplate.hpp"

void BasicTemplate::mouseEventHandler(Window *sender, const MouseRecord mouseRecord)
{
    mouseEvent.call(sender, mouseRecord);
}

void BasicTemplate::keyEventHandler(Window *sender, const KeyRecord keyRecord)
{
    keyEvent.call(sender, keyRecord);
}
