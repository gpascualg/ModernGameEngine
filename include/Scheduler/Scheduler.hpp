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

#define MAX_SKIP_FRAMES 5

namespace Core {

    class Updater;

    struct Ticker
    {
        Updater* updater;
        int ticks;
        int accumulated;
    };

	struct SyncStruct
	{
		std::function<void(void*)> function;
		void* arg;
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
        LFS_INLINE void sync(std::function<void(void*)> func, void* arg = nullptr);

        LFS_INLINE int update();

        static int update_handler(void* me);

		void setUpdateEndCallback(std::function<void(void*, float)> fnc, void* arg)
		{
			_updateEndCallback = fnc;
			_updateEndArgument = arg;
		}

	signals:
		SIGNAL(updateEnd(float));

    private:
        Scheduler(int ticksPerSecond);

    private:
        static Scheduler<TimeBase>* _instance;

        int _ticksPerSecond;
        double _updateEvery;
        uint64_t _lastUpdate;
		uint64_t _nextTick;
        std::vector<Ticker> _timers;
		std::vector<Pool::Future> _futures;
        moodycamel::ConcurrentQueue<SyncStruct> _syncExec;

		std::function<void(void*, float)> _updateEndCallback;
		void* _updateEndArgument;
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
		using Clock = std::chrono::high_resolution_clock;
		auto duration = Clock::now().time_since_epoch();
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
    void Scheduler<T>::sync(std::function<void(void*)> func, void* arg)
    {
		_syncExec.enqueue(SyncStruct{ func, arg });
    }

	template <typename T>
	int Scheduler<T>::update()
	{
		//uint64_t time_now = now();
		uint8_t loops = 0;
		float interpolate = 0;

		// Remaining ticks since last update
		while (now() > _nextTick && loops < MAX_SKIP_FRAMES)
		{
			for (size_t i = 0; i < _timers.size(); ++i)
			{
				auto& ticker = _timers[i];

				if (ticker.accumulated + 1 >= ticker.ticks)
				{
					_futures.push_back(Pool::ThreadPool::get()->enqueue(
						Pool::Function(ticker.updater->entry_point),
						ticker.updater));

					ticker.accumulated = 0;
				}
				else
				{
					++ticker.accumulated;
				}
			}

			_nextTick += (uint64_t)_updateEvery;
			++loops;

			// Wait for all tasks to finish
			for (size_t i = 0; i < _futures.size(); ++i)
			{
				_futures[i].get();
			}
			_futures.clear();

			// One time synchronized executions
			size_t dequeueCount;
			SyncStruct fns[5];
			while ((dequeueCount = _syncExec.try_dequeue_bulk(fns, 5)) > 0)
			{
				for (unsigned int i = 0; i < dequeueCount; ++i)
				{
					SyncStruct& temp = fns[i];
					temp.function(temp.arg);
					--dequeueCount;
				}
			}
		}

		interpolate = float(now() - _nextTick + _updateEvery) / float(_updateEvery); // 10 -/-/-/-/-/-/-> 11
		//emit(this, &Scheduler::updateEnd, interpolate);
		_updateEndCallback(_updateEndArgument, interpolate);

		return 1;
	}

	/*
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
	*/
}
