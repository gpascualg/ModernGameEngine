#include "Scheduler.hpp"

namespace Core {
    template <typename T>
    Scheduler<T>* Scheduler<T>::_instance = nullptr;

    template <typename T>
    Scheduler<T>::Scheduler(int ticksPerSecond):
        _ticksPerSecond(ticksPerSecond),
        _lastUpdate(now()),
        _nextTick(now()),
        _lastCounterReset(now()),
        _FPS(0),
        _ticks(0)
    {
        std::chrono::seconds sec(1);
        _timeDivider = std::chrono::duration_cast<T>(sec).count();

		_updateEvery = static_cast<double>(_timeDivider) / ticksPerSecond,
		_udpateEveryInSeconds = _updateEvery / _timeDivider;

		LOGD("TicksPerSecond = %d (1s = %ld)", _ticksPerSecond, _timeDivider);
		LOGD("UpdateEvery = %3.3f (%0.5lfs)", _updateEvery, _udpateEveryInSeconds);
    }

    template <typename T>
    int Scheduler<T>::update_handler(void* me)
    {
        //std::this_thread::sleep_for(T(100));
        std::this_thread::yield();
        return static_cast<Scheduler*>(me)->update();
    }


    template Scheduler<std::chrono::milliseconds>::Scheduler(int ticksPerSecond);
    template Scheduler<std::chrono::microseconds>::Scheduler(int ticksPerSecond);
    template Scheduler<std::chrono::nanoseconds>::Scheduler(int ticksPerSecond);

    template Scheduler<std::chrono::milliseconds>* Scheduler<std::chrono::milliseconds>::_instance;
    template Scheduler<std::chrono::microseconds>* Scheduler<std::chrono::microseconds>::_instance;
    template Scheduler<std::chrono::nanoseconds>* Scheduler<std::chrono::nanoseconds>::_instance;

    template int Scheduler<std::chrono::milliseconds>::update_handler(void* me);
    template int Scheduler<std::chrono::microseconds>::update_handler(void* me);
    template int Scheduler<std::chrono::nanoseconds>::update_handler(void* me);
}
