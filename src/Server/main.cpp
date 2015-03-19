#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

#include "core.h"
#include "counter.h"

#include "Broadcast.h"
#include "Game.h"
#include "Pool.h"
#include "Scheduler.h"
#include "ThreadPool.h"
#include "Updater.h"
#include "Window.h"

using Scheduler = Core::Scheduler<time_base>;

Pool::ThreadPool* threadPool = nullptr;
Scheduler* scheduler = nullptr;

Game* game = nullptr;

int main() {

    // Setup pool and schedule
    threadPool = Pool::ThreadPool::create(0);
    scheduler = Scheduler::create(100);

    // Setup window and scene
    Window window(640, 480, "CubGPU");
    game = new Game(&window);
    scheduler->every(50, game);

    // Setup signal bindings
    bind(&window, &Window::initializeGL, game, &Game::initializeGL);
    bind(&window, &Window::draw, game, &Game::draw);
    bind(&window, &Window::resize, game, &Game::onResize);
    bind(&window, &Window::mousemove, game, &Game::onMouseMove);

    window.mainloop(scheduler);

    printf("[END] Stopping all threads\n");

    threadPool->stop();
    threadPool->join();

    printf("[END] Cleaning memory\n");
    
    unbindAll();

    delete game;
    delete scheduler;
    delete threadPool;

    return 0;
}
