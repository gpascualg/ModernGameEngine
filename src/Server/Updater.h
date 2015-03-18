#pragma once

#include <atomic>

#include "concurrentqueue.h"
#include "Pool.h"

namespace Core
{
	class Updater;

	template <typename T>
	class Scheduler;

	using Function = std::function<int(Updater*)>;
	using Queue = moodycamel::ConcurrentQueue<Function>;
	using ProducerToken = moodycamel::ProducerToken;
	using ConsumerToken = moodycamel::ConsumerToken;

	class Updater
	{
		template <typename T>
		friend class Scheduler;

	public:
		virtual ~Updater()
		{
			delete [] _functions;
		}

	protected:
		Updater()
		{
		}

		Pool::Future addToPool();

		virtual int update()
		{
			
		}

	private:
		static int entry_point(void* me)
		{
			return static_cast<Updater*>(me)->update();
		}

	protected:
		Queue* _queue;
		int _bulkDequeue;
		Function* _functions;
	};
}
