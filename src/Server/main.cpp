#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

#include "core.h"
#include "counter.h"

#include "Broadcast.h"
#include "Pool.h"
#include "Scheduler.h"
#include "ThreadPool.h"
#include "Updater.h"
#include "Window.h"

Counter c;

class Scene : public Core::Updater
{
public:
	Scene(Window* window) :
		Updater(),
		_window(window)
	{}

	virtual int update() override
	{
		//printf("%d\n", c.inc());

		return Updater::update();
		//std::cout << "ID: " << std::this_thread::get_id() << std::endl;
	}

	void onResize(int width, int height)
	{
		int side = std::min(width, height);
	    glViewport((width - side) / 2, (height - side) / 2, side, side);

	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	#ifdef QT_OPENGL_ES_1
	    glOrthof(-1.5, +1.5, -1.5, +1.5, 0.0, 15.0);
	#else
	    glOrtho(-1.5, +1.5, -1.5, +1.5, 0.0, 15.0);
	#endif
	    glMatrixMode(GL_MODELVIEW);

		printf("Called %dx%d\n", width, height);
		//_window->update();
	}

private:
	Window* _window;
};

using Scheduler = Core::Scheduler<time_base>;

Pool::ThreadPool* threadPool = nullptr;
Scheduler* scheduler = nullptr;

Scene* scene = nullptr;

int main() {

	threadPool = Pool::ThreadPool::create(4);
	scheduler = Scheduler::create(100);

	//threadPool->permanent(Pool::Function(Scheduler::update_handler), scheduler);
	Window window(640, 480, "CubGPU");

	// Setup signals
	Broadcast<int, int>::bind(&window, &Window::resize, scene, &Scene::onResize);

	// Setup scene
	scene = new Scene(&window);
	scheduler->every(50, scene);


	window.mainloop(scheduler);

	printf("[END] Stopping all threads\n");

	threadPool->stop();
	threadPool->join();

	printf("[END] Cleaning memory\n");

	delete scene;
	delete scheduler;
	delete threadPool;

	return 0;
}
