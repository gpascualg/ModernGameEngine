#include "Scheduler.h"

#include "Updater.h"

namespace Core {
    template <typename T>
    Scheduler<T>* Scheduler<T>::_instance = nullptr;

    template <typename T>
    int Scheduler<T>::update()
    {
        uint64_t time_now = now();
        uint64_t elapsed = time_now - _lastUpdate;
        double ticksElapsed = elapsed / _updateEvery;

        if (ticksElapsed >= 1)
        {
            int realTicksElapsed = static_cast<int>(ticksElapsed);
            double lambdaTime = (ticksElapsed - realTicksElapsed) * _updateEvery;

            for (auto& ticker : _timers)
            {
                if (ticker.accumulated + realTicksElapsed >= ticker.ticks)
                {
                    ticker.updater->addToPool();
                    ticker.accumulated = 0;
                }
                else
                {
                    ticker.accumulated += realTicksElapsed;
                }
            }

            _lastUpdate = time_now - static_cast<uint64_t>(lambdaTime);
        }

        size_t dequeueCount;
        std::function<void(void)> fns[5];

        while ((dequeueCount = _singleExec.try_dequeue_bulk(fns, 5)) > 0)
        {
            for (auto fnc : fns)
            {
                if (!dequeueCount)
                {
                    break;
                }

                fnc();
                --dequeueCount;
            }
        }

        return 1;
    }


    template Scheduler<std::chrono::milliseconds>* Scheduler<std::chrono::milliseconds>::_instance;
    template Scheduler<std::chrono::microseconds>* Scheduler<std::chrono::microseconds>::_instance;

    template int Scheduler<std::chrono::milliseconds>::update();
    template int Scheduler<std::chrono::microseconds>::update();
}
