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
	LFS_INLINE void setMouseFixed();
	LFS_INLINE void setMouseFixed(double x, double y);

	LFS_INLINE int getWidth();
	LFS_INLINE int getHeight();

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

	int _width;
	int _height;

	bool _fakeCallback;
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

LFS_INLINE int Window::getWidth()
{
	return _width;
}

LFS_INLINE int Window::getHeight()
{
	return _height;
}
