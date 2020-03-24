#ifndef NUMERICBOX_HPP
#define NUMERICBOX_HPP

#include "InputBox.hpp"

class NumericBox : public InputBox
{
public:
    NumericBox(const Window *parent, const short x, const short y, const short width, const short height);
    double getNumber() const;
    void setNumber(const double num);
private:
    using InputBox::getText;
    using InputBox::setText;

    void keyEventHandler(Window *sender, const KeyRecord keyRecord) override;
};

#endif // NUMERICBOX_HPP
