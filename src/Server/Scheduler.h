#pragma once

#include <cassert>
#include <chrono>
#include <deque>
#include <iostream>
#include <queue>
#include <thread>

#include "core.h"


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
		static Scheduler<TimeBase>* create(int ticksPerSecond)
		{
			if (!_instance)
			{
				_instance = new Scheduler(ticksPerSecond);
			}
			return get();
		}

		static Scheduler<TimeBase>* get()
		{
			assert(_instance && "No Scheduler created");
			return _instance;
		}

		LFS_INLINE uint64_t now()
		{
			auto now = std::chrono::system_clock::now();
			auto duration = now.time_since_epoch();
			auto count = std::chrono::duration_cast<TimeBase>(duration).count();
			return count;
		}

		LFS_INLINE void every(int ticks, Updater* updater)
		{
			_timers.push_back(Ticker{updater, ticks, 0});
		}

		int update();

		static int update_handler(void* me)
		{
			std::this_thread::sleep_for(TimeBase(100));
			return static_cast<Scheduler*>(me)->update();
		}

	private:
		Scheduler(int ticksPerSecond):
			_ticksPerSecond(ticksPerSecond),
			_lastUpdate(now())
		{
			std::chrono::seconds sec(1);
			auto base = std::chrono::duration_cast<TimeBase>(sec).count();

			_updateEvery = static_cast<double>(base) / ticksPerSecond,

			printf("[DEBUG]\tTicksPerSecond = %d\n\tUpdateEvery = %3.3f (%0.5lfs)\n", _ticksPerSecond, _updateEvery, _updateEvery / base);
		}

	private:
		static Scheduler<TimeBase>* _instance;

		int _ticksPerSecond;
		double _updateEvery;
		uint64_t _lastUpdate;
		std::vector<Ticker> _timers;
	};
}
