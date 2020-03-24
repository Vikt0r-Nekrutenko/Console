#include "Window.hpp"
#include "InputBox.hpp"
#include <vector>
#include <iostream>

using namespace std;

class my_window : public Window {
public:
    my_window()
        : Window(),
          box1{ new InputBox(this,  0, 0, 10, 5) },
          box2{ new InputBox(this, 12, 0, 10, 4) }
    {
        m_controls.push_back(box1);
        m_controls.push_back(box2);

        box1->mouseEvent = TMouseEventHandler(&my_window::onBox1Clicked);
        box2->mouseEvent = TMouseEventHandler(&my_window::onBox2Clicked);

        box1->show();
        box2->show();
    }
    ~my_window() override
    {
        delete box2;
        delete box1;
    }

    void onBox1Clicked(const MouseRecord rect) { }

    void onBox2Clicked(const MouseRecord rect) {
        if (rect.button == MouseButton::RIGHT && rect.isPressed)
        {

        } else if (rect.button == MouseButton::LEFT && rect.isPressed) {
            int w = std::atoi(box1->getText().c_str());
            int h = std::atoi(box2->getText().c_str());
            setFontParams(w, h);
        }
    }

    InputBox *box1, *box2;
};

int main()
{
    my_window wnd;
    windowEventProc(&wnd);
    return 0;
}
