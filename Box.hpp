#ifndef BOX_HPP
#define BOX_HPP

#include "BasicTemplate.hpp"
#include <string>

enum class Color
{
    FG_BLACK        = 0U,
    FG_DARK_BLUE    = 1U,
    FG_DARK_GREEN   = 2U,
    FG_DARK_CYAN    = 3U,
    FG_DARK_RED     = 4U,
    FG_DARK_MAGENTA = 5U,
    FG_DARK_YELLOW  = 6U,
    FG_GREY         = 7U,
    FG_DARK_GREY    = 8U,
    FG_BLUE         = 9U,
    FG_GREEN        = 10U,
    FG_CYAN         = 11U,
    FG_RED          = 12U,
    FG_MAGENTA      = 13U,
    FG_YELLOW       = 14U,
    FG_WHITE        = 15U,

    BG_DARK_BLUE    = 16U,
    BG_DARK_GREEN   = 32U,
    BG_DARK_CYAN    = 48U,
    BG_DARK_RED     = 64U,
    BG_DARK_MAGENTA = 80U,
    BG_DARK_YELLOW  = 96U,
    BG_GREY         = 112U,
    BG_DARK_GREY    = 128U,
    BG_BLUE         = 144U,
    BG_GREEN        = 160U,
    BG_CYAN         = 176U,
    BG_RED          = 192U,
    BG_MAGENTA      = 208U,
    BG_YELLOW       = 224U,
    BG_WHITE        = 230U,
};

constexpr unsigned char UNDEF_SYMBOL { 0 };

class Window;

class Box : public BasicTemplate
{
    friend void windowEventProc(Window *window);
public:
    Box(const Window *parent, const short x, const short y, const short width, const short height, const std::string title = "Box");
    ~Box() override;

    virtual void show();
    virtual void activate();
    virtual void deactivate();
    virtual void fill(const Color color = Color::FG_BLACK, const char symbol = UNDEF_SYMBOL);
    void resize(const short width, const short height) override;
    void move(const short x, const short y) override;
    void setTitle(const std::string title);
    const SMALL_RECT &getFrame() const;

    using BasicTemplate::keyEvent;
    using BasicTemplate::mouseEvent;
protected:
    CHAR_INFO &get(const short x, const short y);

    SMALL_RECT  m_frame;
    std::string m_title;
    COORD       m_size;
    Color       m_color;
    CHAR_INFO   *m_rect;
    Window      *m_parent;
};

#endif // BOX_HPP
