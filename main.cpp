#include "Window.hpp"
#include "OutputBox.hpp"
#include "NumericBox.hpp"
#include <vector>
#include <iostream>

using namespace std;

class my_window : public Window {
public:
    my_window()
        : Window(),
          box1{ new InputBox(this,  0, 0, 10, 5) },
          box2{ new NumericBox(this, 12, 0, 10, 5) },
          box3{ new OutputBox(this, 24, 0, 10, 5) }
    {
        m_controls.push_back(box1);
        m_controls.push_back(box2);
        m_controls.push_back(box3);

        box1->mouseEvent = TMouseEventHandler(&my_window::onBox1Clicked);
        box2->mouseEvent = TMouseEventHandler(&my_window::onBox2Clicked);

        box1->setText("some text and something else");
        box1->show();
        box2->show();
        box3->show();
    }
    ~my_window() override
    {
        delete box3;
        delete box2;
        delete box1;
    }

    void onBox1Clicked(const MouseRecord rect) {
        if (rect.button == MouseButton::RIGHT && rect.isPressed) {
            box3->setText(box1->getText());
        }
    }
    void onBox2Clicked(const MouseRecord rect) {
        if (rect.button == MouseButton::RIGHT && rect.isPressed) {
            box3->setText(std::to_string(box2->getNumber() + 1.0));
        }
    }

    InputBox *box1;
    NumericBox *box2;
    OutputBox *box3;
};

int main()
{
    my_window wnd;
    windowEventProc(&wnd);

    return 0;
}
