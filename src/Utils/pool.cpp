#include "pool.h"

namespace Pool {

	ThreadPool* ThreadPool::_instance = nullptr;

	void TaskWorker::_functor()
	{
		TaskWrapper** wrapper = new TaskWrapper*[_bulkDequeue];
		size_t dequeueCount = 0;

		while (1)
		{
			while (!this->_stop) {
				if ((dequeueCount = _queue->try_dequeue_bulk(wrapper, _bulkDequeue)) > 0)
				{
					break;
				}

				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}

			if (this->_stop) return;

			this->_busy = true;
			for (size_t i = 0; i < dequeueCount; ++i)
			{
				wrapper[i]->task(wrapper[i]->argument);
                delete wrapper[i];
			}
			this->_busy = false;
		}

		delete [] wrapper;
	}

	void PermaWorker::_functor()
    {
		while (!_stop)
		{
            _busy = true;
				_function(_argument);
			_busy = false;
		}
	}
}