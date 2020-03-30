#include "Renderer.hpp"

Renderer::Renderer(const Window *parent, const short x, const short y, const short width, const short height, const std::string title)
    : Box(parent, x, y, width, height, title)
{

}

void Renderer::fill(const Color color, const char symbol)
{
    for (int i = width() * height() - 1; i >= 0; i--) {
        int x = i % width() + 1;
        int y = i / width() + 1;
        size_t indx = m_size.X * y + x;
        m_rect[indx].Char.AsciiChar = symbol;
        m_rect[indx].Attributes = WORD(color);
    }
}
void Renderer::putPixel(const int x, const int y, const char symb, const Color col)
{
    size_t indx = m_size.X * (y + 1) + (x + 1);
    m_rect[indx].Char.AsciiChar = symb;
    m_rect[indx].Attributes = WORD(col);
}
