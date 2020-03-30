#include "Window.hpp"
#include "OutputBox.hpp"
#include "NumericBox.hpp"
#include "Renderer.hpp"
#include <vector>
#include <iostream>

using namespace std;

double fRand(double fMin, double fMax) {
    return fMin + (double(rand()) / RAND_MAX) * (fMax - fMin);
}

class my_window : public Window {
public:
    my_window()
        : Window(),
          box1{ new NumericBox(this,  0, 0, 10, 5) },
          box2{ new NumericBox(this, 12, 0, 10, 5) },
          box3{ new OutputBox(this, 24, 0, 10, 5) },
          renderer { new Renderer(this, 0, 6, 50, 30) }
    {
        setFontParams(8, 8);
        addNewControl({box1, box2, box3, renderer});

        box1->mouseEvent = TMouseEventHandler(&my_window::onBox1Clicked);
        box2->mouseEvent = TMouseEventHandler(&my_window::onBox2Clicked);
        box3->mouseEvent = TMouseEventHandler(&my_window::onBox3Clicked);
    }
    ~my_window() override
    {
        delete box3;
        delete box2;
        delete box1;
    }

    void onBox1Clicked(const MouseRecord rect) {
        if (rect.button == MouseButton::RIGHT && rect.isPressed) {
            box1->setNumber(fRand(10, 100));
        }
    }
    void onBox2Clicked(const MouseRecord rect) {
        if (rect.button == MouseButton::RIGHT && rect.isPressed) {
            box2->setNumber(fRand(10, 100));
        }
    }
    void onBox3Clicked(const MouseRecord rect) {
        if (rect.button == MouseButton::RIGHT && rect.isPressed) {
            box3->setText(std::to_string(box2->getNumber() + box1->getNumber()));
        }
    }
    void update(const float deltaTime) override {
        renderer->fill();
        renderer->putPixel(rand() % renderer->width(),
                           rand() % renderer->height(),
                           '#', Color::FG_WHITE);
        renderer->show();
    }
    NumericBox *box1;
    NumericBox *box2;
    OutputBox *box3;
    Renderer *renderer = nullptr;
};

int main()
{
    my_window wnd;
    windowEventProc(&wnd);

    return 0;
}
