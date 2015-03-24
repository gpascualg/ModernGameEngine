#pragma once

#include "Updater.hpp"
#include "core.hpp"

class Window;
class Object;

namespace Shader
{
    class Program;
}

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
    Shader::Program* _program;

    Object* _cube;

    double xRot;
    double yRot;
    double zRot;

    double lastX;
    double lastY;
};
