#include "Box.hpp"
#include "Window.hpp"

Box::Box(const Window *parent, const short x, const short y, const short width, const short height)
    : m_color(Color::DARK_BLUE),
      m_rect(nullptr),
      m_parent { const_cast<Window *>(parent) }
{
    m_size.X = width  < 3 ? 3 : width;
    m_size.Y = height < 3 ? 3 : height;

    m_frame = {x, y, short(x + m_size.X), short(y + m_size.Y)};

    resize(m_size.X, m_size.Y);
}

Box::~Box()
{
    delete [] m_rect;
}

CHAR_INFO &Box::get(const short x, const short y)
{
    return m_rect[m_size.X * y + x];
}

void Box::resize(const short width, const short height)
{
    if (m_rect != nullptr) {
        fill(); show();
        delete [] m_rect;
    }

    m_size.X = width  < 3 ? 3 : width;
    m_size.Y = height < 3 ? 3 : height;

    m_rect = new CHAR_INFO[m_size.X * m_size.Y];
    fill(m_color, ' ');

    for (short y = 0; y < m_size.Y; y++) {
        for (short x = 0; x < m_size.X; x++) {
            if (y == 0 || y == m_size.Y - 1) {
                get(x, y).Char.AsciiChar = char(196); // -
            }
            if (x == 0 || x == m_size.X - 1) {
                get(x, y).Char.AsciiChar = char(179); // |
            }
        }
    }

    get(0, 0).Char.AsciiChar                        = char(218); // „¡
    get(m_size.X - 1, 0).Char.AsciiChar             = char(191); // „¢
    get(0, m_size.Y - 1).Char.AsciiChar             = char(192); // „¤
    get(m_size.X - 1, m_size.Y - 1).Char.AsciiChar  = char(217); // „£
}

void Box::move(const short x, const short y)
{
    fill(); show();
    m_frame = { x, y, short(x + m_size.X), short(y + m_size.Y) };
    fill(m_color); show();
}

void Box::fill(const Color color, const char symbol)
{
    for (int i = m_size.X * m_size.Y - 1; i >= 0; --i) {
        m_rect[i].Char.AsciiChar = (symbol == UNDEF_SYMBOL) ? m_rect[i].Char.AsciiChar : symbol;
        m_rect[i].Attributes = WORD(color);
    }
}

void Box::show()
{
    WriteConsoleOutputA(m_parent->out(), m_rect, m_size, { 0, 0 }, &m_frame);
}

void Box::activate()
{
    m_color = Color(WORD(m_color) | FOREGROUND_INTENSITY); fill(m_color); show();
}

void Box::deactivate()
{
    m_color = Color(WORD(m_color) ^ FOREGROUND_INTENSITY); fill(m_color); show();
}

const SMALL_RECT &Box::getFrame() const
{
    return m_frame;
}

