#pragma once

#include "Updater.h"
#include "core.h"

class Window;

class Game : public Core::Updater
{
public:
    Game(Window* window);

    virtual int update() override;

    void initializeGL();
    void onResize(int width, int height);
    void onMouseMove(double x, double y, uint8_t mouse);
    void draw();

private:
    Window* _window;

    double xRot;
    double yRot;
    double zRot;

    double lastX;
    double lastY;
};
