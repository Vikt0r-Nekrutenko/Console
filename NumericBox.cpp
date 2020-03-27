#include "NumericBox.hpp"

NumericBox::NumericBox(const Window *parent, const short x, const short y, const short width, const short height)
    : InputBox(parent, x, y, width, height)
{

}

double NumericBox::getNumber() const
{
    return std::atof(m_text.c_str());
}

void NumericBox::setNumber(const double num)
{
    InputBox::setText(std::to_string(num));
}

void NumericBox::keyEventHandler(Window *sender, const KeyRecord keyRecord)
{
    if (keyRecord.isPressed && !isControlKeyPressed(keyRecord.keyCode)) {
        if ((keyRecord.symbol >= '0' && keyRecord.symbol <= '9') ||
            (keyRecord.symbol == '.' && m_text.find('.') == std::string::npos) ||
            (keyRecord.symbol == '-' && m_symbolInText == 0 && m_text.find('-') == std::string::npos))
        {
            m_text.insert(m_symbolInText, 1, keyRecord.symbol);

            scrollRight();
            show();
        }
    }
    keyEvent.call(sender, keyRecord);
}
