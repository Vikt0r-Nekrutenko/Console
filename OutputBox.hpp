#ifndef OUTPUTBOX_HPP
#define OUTPUTBOX_HPP

#include "InputBox.hpp"

class OutputBox : public InputBox
{
public:
    OutputBox(const Window *parent, const short x, const short y, const short width = 10, const short height = 3, const std::string title = "Output");
private:
    void keyEventHandler(Window *sender, const KeyRecord keyRecord) override;
    bool isControlKeyPressed(unsigned short key) override;
};

#endif // OUTPUTBOX_HPP
