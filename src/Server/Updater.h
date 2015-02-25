#pragma once

#include <atomic>

#include "concurrentqueue.h"
#include "pool.h"

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
		Updater(Queue* queue, int bulkDequeue):
			_queue(queue),
			_bulkDequeue{bulkDequeue}
		{
			_functions = new Function[_bulkDequeue];
		}

		Pool::Future addToPool()
		{
			return Pool::ThreadPool::get()->enqueue(Pool::Function(entry_point), this);
		}

		bool queue_task(ProducerToken& token, Function function)
		{
			return _queue->enqueue(token, function);
		}

		virtual int update()
		{
			return updateQueue();
		}

	private:
		int updateQueue()
		{
			size_t count = _queue->try_dequeue_bulk(_functions, _bulkDequeue);
			for (size_t i = 0; i < count; ++i)
			{
				_functions[i](this);
			}

            return 1;
		}

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
