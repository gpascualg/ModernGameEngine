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

class Window
{
    template<typename ... T>
    friend class Broadcast;

public:
    Window(uint32_t width, uint32_t height, const char* title);

    void mainloop();
    LFS_INLINE void update();

	GLFWwindow* operator*()
	{
		return _window;
	}

signals:
    SIGNAL(initializeGL());
    SIGNAL(draw());
    SIGNAL(resize(int, int));
    SIGNAL(mousemove(double, double, uint8_t));

private:
    static void _resizeHandler(GLFWwindow* w, int width, int height);
    static void _refreshHandler(GLFWwindow* w);
    static void _cursorPosHandler(GLFWwindow* w, double x, double y);

private:
    GLFWwindow* _window;
    Scheduler* _scheduler;
    std::atomic<bool> _update;

    static std::map<uintptr_t, Window*> _windowToThis;
};

void Window::update()
{
    _update = true;
}
