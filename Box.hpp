#ifndef BOX_HPP
#define BOX_HPP

#include "BasicTemplate.hpp"

enum class Color
{
    BLACK        = 0U,
    DARK_BLUE    = 1U,
    DARK_GREEN   = 2U,
    DARK_CYAN    = 3U,
    DARK_RED     = 4U,
    DARK_MAGENTA = 5U,
    DARK_YELLOW  = 6U,
    GREY         = 7U,
    DARK_GREY    = 8U,
    BLUE         = 9U,
    GREEN        = 10U,
    CYAN         = 11U,
    RED          = 12U,
    MAGENTA      = 13U,
    YELLOW       = 14U,
    WHITE        = 15U
};

constexpr unsigned char UNDEF_SYMBOL { 0 };

class Window;

class Box : public BasicTemplate
{
    friend void windowEventProc(Window *window);
public:
    Box(const Window *parent, const short x, const short y, const short width, const short height);
    ~Box() override;

    virtual void show();
    virtual void activate();
    virtual void deactivate();
    void resize(const short width, const short height) override;
    void move(const short x, const short y) override;
    void fill( const Color color = Color::BLACK, const char symbol = UNDEF_SYMBOL);

    const SMALL_RECT &getFrame() const;

    using BasicTemplate::keyEvent;
    using BasicTemplate::mouseEvent;
protected:
    CHAR_INFO &get(const short x, const short y);

    SMALL_RECT m_frame;
    COORD      m_offset = { 0, 0 };
    COORD      m_size;
    COORD      m_position;
    Color      m_color;
    CHAR_INFO *m_rect;
    Window    *m_parent;
};

#endif // BOX_HPP
