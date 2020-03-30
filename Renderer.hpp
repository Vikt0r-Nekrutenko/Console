#ifndef RENDERER_H
#define RENDERER_H

#include <Box.hpp>

class Renderer : public Box
{
public:
    Renderer(const Window *parent, const short x, const short y, const short width, const short height, const std::string title = "Renderer");
    void fill(const Color color = Color::FG_BLACK, const char symbol = UNDEF_SYMBOL) override;
    void putPixel(const int x, const int y, const char symb, const Color col);
    inline short width() const { return m_size.X - 2; }
    inline short height() const { return m_size.Y - 2; }
};

#endif // RENDERER_H
