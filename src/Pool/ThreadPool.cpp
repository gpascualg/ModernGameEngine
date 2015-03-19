#include "ThreadPool.h"

#include "WorkerThread.h"
#include "TaskWorker.h"
#include "PermaWorker.h"
#include "TaskWrapper.h"


namespace Pool {

    ThreadPool* ThreadPool::_instance = nullptr;

    ThreadPool::ThreadPool(int nThreads, int bulkDequeue):
        _max{nThreads},
        _bulkDequeue{bulkDequeue},
        _token(_queue)
    {
        for (int i = 0; i < nThreads; ++i)
        {
            TaskWorker* worker = new TaskWorker(&_queue, _bulkDequeue);
            worker->start();
            _workers.push_back(worker);
        }
    }

    ThreadPool::~ThreadPool()
    {
        for (WorkerThread* worker : _workers)
        {
            assert(worker->_stop && !worker->_busy);

            delete worker;
        }   
    }

    Future ThreadPool::enqueue(Function&& function, void* argument)
    {
        Task task(function);
        Future future = task.get_future();
        _queue.enqueue(_token, new TaskWrapper(task, argument));
        
        return future;
    }

    void ThreadPool::permanent(Function&& function, void* argument)
    {
        PermaWorker* worker = new PermaWorker(function, argument);
        worker->start();
        _workers.push_back(worker);
    }

    void ThreadPool::stop()
    {
        for (WorkerThread* worker : _workers)
        {
            worker->_stop = true;
        }
    }

    void ThreadPool::join()
    {
        for (WorkerThread* worker : _workers)
        {
            worker->_thread.join();
        }   
    }
}
