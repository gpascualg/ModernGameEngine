#pragma once

#include <cassert>
#include <chrono>
#include <deque>
#include <iostream>
#include <queue>
#include <thread>

#include "core.hpp"
#include "concurrentqueue.hpp"
#include "log.hpp"

#include "Pool.hpp"
#include "ThreadPool.hpp"
#include "Updater.hpp"

namespace Core {

    class Updater;

    struct Ticker
    {
        Updater* updater;
        int ticks;
        int accumulated;
    };

    template <typename TimeBase>
    class Scheduler
    {
    public:
        static LFS_INLINE Scheduler<TimeBase>* create(int ticksPerSecond);
        static LFS_INLINE Scheduler<TimeBase>* get();

        LFS_INLINE uint64_t now();

        LFS_INLINE void every(int ticks, Updater* updater);
        LFS_INLINE Pool::Future async(Pool::Function& fnc, void* arg);
        LFS_INLINE void sync(std::function<void(void)> func);

        LFS_INLINE int update();

        static int update_handler(void* me);

    private:
        Scheduler(int ticksPerSecond);

    private:
        static Scheduler<TimeBase>* _instance;

        int _ticksPerSecond;
        double _updateEvery;
        uint64_t _lastUpdate;
        std::vector<Ticker> _timers;
        moodycamel::ConcurrentQueue <std::function<void(void)> > _syncExec;
    };


    // INLINE METHODS

    template <typename T>
    Scheduler<T>* Scheduler<T>::create(int ticksPerSecond)
    {
        if (!_instance)
        {
            _instance = new Scheduler(ticksPerSecond);
        }
        return get();
    }

    template <typename T>
    Scheduler<T>* Scheduler<T>::get()
    {
        assert(_instance && "No Scheduler created");
        return _instance;
    }

    template <typename T>
    uint64_t Scheduler<T>::now()
    {
        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        auto count = std::chrono::duration_cast<T>(duration).count();
        return count;
    }

    template <typename T>
    void Scheduler<T>::every(int ticks, Updater* updater)
    {
        _timers.push_back(Ticker{updater, ticks, 0});
    }

    template <typename T>
    Pool::Future Scheduler<T>::async(Pool::Function& fnc, void* arg)
    {
        return Pool::ThreadPool::get()->enqueue(std::move(fnc), arg);
    }

    template <typename T>
    void Scheduler<T>::sync(std::function<void(void)> func)
    {
        _syncExec.enqueue(func);
    }

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
                    Pool::ThreadPool::get()->enqueue(
                        Pool::Function(ticker.updater->entry_point),
                        ticker.updater);

                    ticker.accumulated = 0;
                }
                else
                {
                    ticker.accumulated += realTicksElapsed;
                }
            }

            _lastUpdate = time_now - static_cast<uint64_t>(lambdaTime);
        }

        // One time synchronized executions
        size_t dequeueCount;
        std::function<void(void)> fns[5];
        while ((dequeueCount = _syncExec.try_dequeue_bulk(fns, 5)) > 0)
        {
            for (unsigned int i = 0; i < dequeueCount; ++i)
            {
                fns[i]();
                --dequeueCount;
            }
        }

        return 1;
    }
}
