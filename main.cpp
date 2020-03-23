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
        box2->keyEvent   = TKeyEventHandler(&my_window::onBox2KeyPressed);

        box1->show();
        box2->setText("some text and something else!");
        box2->show();
    }
    ~my_window() override
    {
        delete box2;
        delete box1;
    }

    void onBox2KeyPressed(const KeyRecord kr) { }

    void onBox1Clicked(const MouseRecord rect) { }

    void onBox2Clicked(const MouseRecord rect) {
        if (rect.button == MouseButton::RIGHT && rect.isPressed)
        {
            box2->resize(rand()% 20, rand()% 20);
        } else if (rect.button == MouseButton::LEFT && rect.isPressed) {
            MessageBoxA(m_wnd, box2->getText().c_str(), "", MB_OK);
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
