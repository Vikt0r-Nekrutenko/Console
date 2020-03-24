#ifndef OUTPUTBOX_HPP
#define OUTPUTBOX_HPP

#include "InputBox.hpp"

class OutputBox : public InputBox
{
public:
    OutputBox(const Window *parent, const short x, const short y, const short width, const short height);
private:
    void keyEventHandler(Window *sender, const KeyRecord keyRecord) override;
    bool isControlKeyPressed(unsigned short key) override;
};

#endif // OUTPUTBOX_HPP