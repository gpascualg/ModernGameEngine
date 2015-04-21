#include "Window.hpp"
#include "Scheduler.hpp"

std::map<uintptr_t, Window*> Window::_windowToThis;

Window::Window(uint32_t width, uint32_t height, const char* title) :
    _window(nullptr),
    _update(false),
    _height(height),
    _width(width),
    _fixMouse(false),
	_xScroll(0),
	_yScroll(0)
{
    static bool libraryInitialized = false;
    if (!libraryInitialized)
    {
        if (!glfwInit())
        {
            return;
        }
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
    glfwSwapInterval(0);

    /* Setup callbacks */
    glfwSetWindowSizeCallback(_window, _resizeHandler);
    glfwSetWindowRefreshCallback(_window, _refreshHandler);
	glfwSetScrollCallback(_window, _scrollHandler);

    /* Key persistance */
    glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(_window, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);

    /* Hide mouse */
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    /* Init Glew */
    if (!libraryInitialized)
    {
        if (glewInit())
        {
            return;
        }
        libraryInitialized = true;
    }
}

void Window::mainloop()
{
    if (!_window)
    {
        return;
    }

    // Save scheduler
    Scheduler* scheduler = Scheduler::get();

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
        scheduler->update();

        /* Poll for and process events */
        glfwPollEvents();
    }
}

void Window::_resizeHandler(GLFWwindow* w, int width, int height)
{
    // We can not immediately emit, it must be scheduled
    Window* window = _windowToThis[(uintptr_t)w];

    window->_width = width;
    window->_height = height;

    Scheduler::get()->sync([window, width, height] (void*) {
        emit(window, &Window::resize, width, height);
    });
}

void Window::_refreshHandler(GLFWwindow* w)
{
    Window* window = _windowToThis[(uintptr_t)w];

    /* Update scheduler */
    Scheduler::get()->update();
}

void Window::_scrollHandler(GLFWwindow* w, double xoffset, double yoffset)
{
	Window* window = _windowToThis[(uintptr_t)w];
	window->_xScroll = xoffset;
	window->_yScroll = yoffset;
}
