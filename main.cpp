#include "Window.hpp"
#include "Box.hpp"
#include <vector>
#include <iostream>

using namespace std;

class my_window : public Window {
public:
    my_window()
        : Window(),
          box1{ new Box(this,  0, 0, 10, 5) },
          box2{ new Box(this, m_placeSize.X - 10, 0, 10, 5) }
    {
        m_controls.push_back(box1);
        m_controls.push_back(box2);

        box1->mouseEvent = TMouseEventHandler(&my_window::onBox1Clicked);
        box2->mouseEvent = TMouseEventHandler(&my_window::onBox2Clicked);
        box2->keyEvent   = TKeyEventHandler(&my_window::onBox2KeyPressed);

        box1->show();
        box2->show();

    }
    ~my_window() override
    {
        delete box2;
        delete box1;
    }

    void onBox2KeyPressed(const KeyRecord kr)
    {
        if (kr.isPressed) {
            box2->fill(Color::RED);
        } else {
            box2->fill(Color::GREEN);
        }
        box2->show();
    }

    void onBox1Clicked(const MouseRecord rect) {

    }

    void onBox2Clicked(const MouseRecord rect) {

    }

    Box *box1, *box2;
};

int main()
{
    my_window wnd;
    windowEventProc(&wnd);
    return 0;
}
