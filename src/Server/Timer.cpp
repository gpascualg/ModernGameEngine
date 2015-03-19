#include "Timer.h"

Timer::Timer()
{}

int Timer::update()
{
    emit(this, &Timer::tick);
    return Updater::update();
}
