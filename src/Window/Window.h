#pragma once

#include "core.h"
#include "Broadcast.h"

#include <functional>
#include <map>

#include <GLFW/glfw3.h>

namespace Core
{
    template<typename T>
    class Scheduler;
}

using Scheduler = Core::Scheduler<time_base>;

class Window
{
    template<typename ... T>
    friend class Broadcast;

public:
    Window(uint32_t width, uint32_t height, const char* title);

    void mainloop(Scheduler* scheduler);

    inline void update()
    {
        _update = true;
    }

signals:
    signal(resize, int, int);
    signal(mousemove, int, int);

private:
    static void _resizeHandler(GLFWwindow* w, int width, int height)
    {
        Broadcast<int, int>::emit(_windowToThis[(uintptr_t)w], &Window::resize, width, height);
    }

private:
    GLFWwindow* _window;
    std::atomic<bool> _update;

    static std::map<uintptr_t, Window*> _windowToThis;
};
