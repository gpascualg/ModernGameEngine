#include <stdio.h>
#include <stdlib.h>

#include "core.hpp"
#include "counter.hpp"
#include "log.hpp"

#include "Broadcast.hpp"
#include "Game.hpp"
#include "Pool.hpp"
#include "Resources.hpp"
#include "Scheduler.hpp"
#include "ThreadPool.hpp"
#include "Updater.hpp"
#include "Window.hpp"

using Scheduler = Core::Scheduler<time_base>;

Pool::ThreadPool* threadPool = nullptr;
Scheduler* scheduler = nullptr;

Game* game = nullptr;


int main(int argc, const char* argv[]) {
    Resources::setup(argv[0], "resources");

    // Setup pool and scheduler
    threadPool = Pool::ThreadPool::create(1);
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

    LOGD("[END] Stopping all threads");

    threadPool->stop();
    threadPool->join();

    LOGD("[END] Cleaning memory");

    unbindAll();

    delete game;
    delete scheduler;
    delete threadPool;

    return 0;
}
