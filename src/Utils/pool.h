#include <atomic>
#include <cmath>
#include <future>
#include <functional>
#include <iostream>
#include <thread>
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

	typedef moodycamel::ConcurrentQueue<TaskWrapper> Queue;
	typedef std::function<int(void*)> Function;
	typedef std::packaged_task<int(void*)> Task;
	typedef std::future<int> Future;


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

		WorkerThread(Queue* queue):
			_thread(_static_functor, this),
			_queue(queue),
			_token(*queue),
			_busy{false},
			_stop{false}
		{
		}

		/*
		LFS_INLINE void setTask(Task&& t) {
			_task = std::move(t);
			_start = true;
		}
		*/

		Queue* _queue;
		moodycamel::ConsumerToken _token;

		std::thread _thread;
		std::atomic<bool> _busy;
		std::atomic<bool> _stop;
	};

	class TaskWorker : public WorkerThread {
		friend ThreadPool;

	private:
		void _functor()
		{
			TaskWrapper wrapper;

			while (1)
			{
				while (!this->_stop) {
					if (_queue->try_dequeue(_token, wrapper))
					{
						break;
					}

					std::this_thread::sleep_for(std::chrono::milliseconds(100));
				}

				if (this->_stop) return;

				this->_busy = true;
					wrapper.task(wrapper.argument);
				this->_busy = false;
			}
		}

		TaskWorker(Queue* queue):
			WorkerThread(queue)
		{
		}
	};

	/*
	class PermaWorker : public WorkerThread {
		friend ThreadPool;

		void _functor()
		{
			while (!this->_start) std::this_thread::sleep_for (std::chrono::milliseconds(100));

			while (1)
			{
				if (this->_stop) return;

				this->_busy = true;
					//this->_task(this->_attribute);
				this->_busy = false;
			}
		}

		PermaWorker(Queue* queue):
			WorkerThread(queue)
		{
		}
	};
	*/

	class ThreadPool {
	private:
		ThreadPool(int nThreads):
			_max{nThreads},
			_token(_queue)
		{
			for (int i = 0; i < nThreads; ++i)
			{
				_workers.push_back(new TaskWorker(&_queue));
			}
		}

	public:
		static ThreadPool* get(int nThreads)
		{
			if (!_instance)
			{
				_instance = new ThreadPool(nThreads);
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
		std::vector<WorkerThread*> _workers;
		
		Queue _queue;
		moodycamel::ProducerToken _token;
	};
};
