#include "Window.h"
#include "Scheduler.h"

std::map<uintptr_t, Window*> Window::_windowToThis;

Window::Window(uint32_t width, uint32_t height, const char* title):
    _window(nullptr),
    _update(false)
{
    static bool libraryInitialized = false;
    if (!libraryInitialized)
    {
        if (!glfwInit())
            return;
    }

    _window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!_window)
    {
        glfwTerminate();
        return;
    }

    /* Save current window */
    _windowToThis[(uintptr_t)_window] = this;

    /* Make the window's context current */
    glfwMakeContextCurrent(_window);

    /* Setup callbacks */
    glfwSetWindowSizeCallback(_window, _resizeHandler);
    glfwSetWindowRefreshCallback(_window, _refreshHandler);
    glfwSetCursorPosCallback(_window, _cursorPosHandler);
}

void Window::mainloop(Scheduler* scheduler)
{
    if (!_window || !scheduler)
    {
        return;
    }

    // Save scheduler
    _scheduler = scheduler;

    // Emit initializeGL
    emit(this, &Window::initializeGL);

    // Emit resize
    int width, height;
    glfwGetWindowSize(_window, &width, &height);
    emit(this, &Window::resize, width, height);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(_window))
    {
        /* Update scheduler */
        _scheduler->update();

        if (_update)
        {
            _update = false;

            /* Call draw */
            emit(this, &Window::draw);
            glfwSwapBuffers(_window);
        }

        /* Poll for and process events */
        glfwPollEvents();
    }
}

void Window::_resizeHandler(GLFWwindow* w, int width, int height)
{
    // We can not immediately emit, it must be scheduled
    Window* window = _windowToThis[(uintptr_t)w];
    window->_scheduler->once([window, width, height] {
        emit(window, &Window::resize, width, height);
    });
}

void Window::_refreshHandler(GLFWwindow* w)
{
    Window* window = _windowToThis[(uintptr_t)w];

    /* Update scheduler */
    window->_scheduler->update();

    /* Call draw */
    emit(window, &Window::draw);
    glfwSwapBuffers(w);
}

void Window::_cursorPosHandler(GLFWwindow* w, double x, double y)
{
    uint8_t mouse = 0;

    if (glfwGetMouseButton(w, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        mouse |= Mouse::LeftButton;
    }
    if (glfwGetMouseButton(w, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        mouse |= Mouse::RightButton;
    }

    // We can not immediately emit, it must be scheduled
    Window* window = _windowToThis[(uintptr_t)w];
    window->_scheduler->once([window, x, y, mouse] {
        emit(window, &Window::mousemove, x, y, mouse);
    });
}
