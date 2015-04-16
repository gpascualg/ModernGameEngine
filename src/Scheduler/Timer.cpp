#include "Timer.hpp"

Timer::Timer()
{}

int Timer::updateCPU(void* arg0)
{
    emit(this, &Timer::tick);
    return Updater::updateCPU(arg0);
}
