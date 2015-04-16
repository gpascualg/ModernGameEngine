#pragma once

#include "core.hpp"
#include "Broadcast.hpp"

#include <functional>
#include <map>

namespace Core
{
    template<typename T>
    class Scheduler;
}

using Scheduler = Core::Scheduler<time_base>;

enum Mouse
{
    LeftButton  = 1,
    RightButton = 2
};

enum Keys
{
    W = GLFW_KEY_W,
    A = GLFW_KEY_A,
    S = GLFW_KEY_S,
    D = GLFW_KEY_D,
    SPACE = GLFW_KEY_SPACE,
    LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT
};

class Window
{
    template<typename ... T>
    friend class Broadcast;

public:
    Window(uint32_t width, uint32_t height, const char* title);

    void mainloop();
    LFS_INLINE void update();
    LFS_INLINE void setMouseFixed();
    LFS_INLINE void setMouseFixed(double x, double y);

    LFS_INLINE int getWidth();
    LFS_INLINE int getHeight();
    LFS_INLINE void getMouseDelta(uint8_t& buttons, double& x, double& y);
    LFS_INLINE bool isKeyPressed(int key);

    GLFWwindow* operator*()
    {
        return _window;
    }

signals:
    SIGNAL(initializeGL());
    SIGNAL(draw());
    SIGNAL(resize(int, int));

private:
    static void _resizeHandler(GLFWwindow* w, int width, int height);
    static void _refreshHandler(GLFWwindow* w);

private:
    GLFWwindow* _window;
    Scheduler* _scheduler;
    std::atomic<bool> _update;

    int _width;
    int _height;

    bool _fixMouse;
    double _fixMouseX;
    double _fixMouseY;

    static std::map<uintptr_t, Window*> _windowToThis;
};

void Window::update()
{
    _update = true;
}

void Window::setMouseFixed()
{
    setMouseFixed(_width / 2.0f, _height / 2.0f);
}

void Window::setMouseFixed(double x, double y)
{
    _fixMouse = true;
    _fixMouseX = x;
    _fixMouseY = y;
}

int Window::getWidth()
{
    return _width;
}

int Window::getHeight()
{
    return _height;
}

void Window::getMouseDelta(uint8_t& buttons, double& x, double& y)
{
    if (glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        buttons |= Mouse::LeftButton;
    }
    if (glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        buttons |= Mouse::RightButton;
    }

    glfwGetCursorPos(_window, &x, &y);
    x -= _fixMouseX;
    y -= _fixMouseY;

    if (_fixMouse)
    {
        glfwSetCursorPos(_window, _fixMouseX, _fixMouseY);
    }
}

bool Window::isKeyPressed(int key)
{
    return glfwGetKey(_window, key) == GLFW_PRESS;
}