#include "Timer.hpp"

Timer::Timer()
{}

int Timer::update()
{
    emit(this, &Timer::tick);
    return Updater::update();
}
