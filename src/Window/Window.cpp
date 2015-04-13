#include "Window.hpp"
#include "Scheduler.hpp"

std::map<uintptr_t, Window*> Window::_windowToThis;

Window::Window(uint32_t width, uint32_t height, const char* title) :
    _window(nullptr),
    _update(false),
	_mouseUpdated(false)
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
    glfwSetCursorPosCallback(_window, _cursorPosHandler);

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

void Window::_cursorPosHandler(GLFWwindow* w, double x, double y)
{
	// We can not immediately emit, it must be scheduled
	Window* window = _windowToThis[(uintptr_t)w];

	if (!window->_mouseUpdated)
	{
		window->_mouseUpdated = true;
		uint8_t mouse = 0;

		if (glfwGetMouseButton(w, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			mouse |= Mouse::LeftButton;
		}
		if (glfwGetMouseButton(w, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			mouse |= Mouse::RightButton;
		}

		emit(window, &Window::mousemove, x, y, mouse);
		window->_mouseUpdated = false;
	}
}
