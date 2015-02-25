#pragma once

#include <atomic>
#include <cmath>
#include <future>
#include <functional>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <vector>


#include "core.h"
#include "concurrentqueue.h"


namespace Pool
{
	class ThreadPool;
	class WorkerThread;

	class TaskWorker;
	class PermaWorker;

	struct TaskWrapper;

	using Queue = moodycamel::ConcurrentQueue<TaskWrapper>;
	using ConsumerToken = moodycamel::ConsumerToken;
	using ProducerToken = moodycamel::ProducerToken;
	using Function = std::function<int(void*)>;
	using Task = std::packaged_task<int(void*)>;
	using Future = std::future<int>;


	struct TaskWrapper
	{
		Task task;
		void* argument;

		TaskWrapper() {}

		TaskWrapper(Task& _task, void* _argument):
			task(std::move(_task)),
			argument(_argument)
		{
		}
	};

	class WorkerThread {
		friend ThreadPool;

	protected:

		virtual void _functor() = 0;

		static void _static_functor(WorkerThread* me) {
			me->_functor();
		}

		WorkerThread(Queue* queue, int bulkDequeue):
			_thread(_static_functor, this),
			_queue(queue),
			_bulkDequeue{bulkDequeue},
			_busy{false},
			_stop{false}
		{
		}

		WorkerThread(nullptr_t n):
			_thread(_static_functor, this),
			_busy{false},
			_stop{false}
		{
		}

		LFS_INLINE std::thread::id getID()
		{
			return std::this_thread::get_id();
		}

		Queue* _queue;

		std::thread _thread;
		int _bulkDequeue;
		std::atomic<bool> _busy;
		std::atomic<bool> _stop;
	};

	class TaskWorker : public WorkerThread {
		friend ThreadPool;

	private:
		void _functor();

		TaskWorker(Queue* queue, int dequeueCount):
			WorkerThread(queue, dequeueCount)
		{
		}
	};

	class PermaWorker : public WorkerThread {
		friend ThreadPool;

	private:
		void _functor();

		PermaWorker(Function& function, void* argument):
			WorkerThread(nullptr),
			_function(std::move(function)),
			_argument(argument)
		{
		}

		Function&& _function;
		void* _argument;
	};

	class ThreadPool {
		friend class WorkerThread;
		friend class TaskWorker;

	private:
		ThreadPool(int nThreads, int bulkDequeue):
			_max{nThreads},
			_bulkDequeue{bulkDequeue},
			_token(_queue)
		{
			for (int i = 0; i < nThreads; ++i)
			{
				TaskWorker* worker = new TaskWorker(&_queue, _bulkDequeue);
				_workers.push_back(worker);
			}
		}

	public:
		static ThreadPool* create(int nThreads, int bulkDequeue = 5)
		{
			if (!_instance)
			{
				_instance = new ThreadPool(nThreads, bulkDequeue);
			}

			return get();
		}

		static ThreadPool* get()
		{
			assert(_instance && "No ThreadPool exists");
			return _instance;
		}

		~ThreadPool() 
		{
			for (WorkerThread* worker : _workers)
			{
				assert(worker->_stop && !worker->_bussy);

				delete worker;
			}	
		}

		LFS_INLINE Future enqueue(Function&& function, void* argument)
		{
			Task task(function);
			Future future = task.get_future();
			_queue.enqueue(_token, TaskWrapper(task, argument));
			
			return future;
		}

		LFS_INLINE void permanent(Function&& function, void* argument)
		{			
			PermaWorker* worker = new PermaWorker(function, argument);
			_workers.push_back(worker);
		}

		LFS_INLINE void stop()
		{
			for (WorkerThread* worker : _workers)
			{
				worker->_stop = true;
			}
		}

		LFS_INLINE void join()
		{
			for (WorkerThread* worker : _workers)
			{
				worker->_thread.join();
			}	
		}

	private:
		static ThreadPool* _instance;

		int _max;
		int _bulkDequeue;
		std::vector<WorkerThread*> _workers;

		Queue _queue;
		ProducerToken _token;
	};
};
