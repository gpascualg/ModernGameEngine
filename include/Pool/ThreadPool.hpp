#pragma once

#include "Pool.hpp"
#include "TaskWrapper.hpp"

namespace Pool {

    class ThreadPool {
        friend class WorkerThread;
        friend class TaskWorker;

    private:
        ThreadPool(int nThreads, int bulkDequeue);

    public:
        static LFS_INLINE ThreadPool* create(int nThreads, int bulkDequeue = 5);
        static LFS_INLINE ThreadPool* get();

        ~ThreadPool();

        LFS_INLINE Future enqueue(Function&& function, void* argument);
        template <class C> LFS_INLINE Future enqueue(int(C::*func)(void*), C* caller, void* argument);
        void permanent(Function&& function, void* argument);

        void stop();
        void join();

    private:
        static ThreadPool* _instance;

        int _max;
        int _bulkDequeue;
        std::vector<WorkerThread*> _workers;

        Queue _queue;
        ProducerToken _token;
    };


    // INLINE METHODS

    ThreadPool* ThreadPool::create(int nThreads, int bulkDequeue)
    {
        if (!_instance)
        {
            _instance = new ThreadPool(nThreads, bulkDequeue);
        }

        return get();
    }

    ThreadPool* ThreadPool::get()
    {
        assert(_instance && "No ThreadPool exists");
        return _instance;
    }

    template <class C> Future ThreadPool::enqueue(int(C::*func)(void*), C* caller, void* argument)
    {
        return enqueue([func, caller](void* arg) -> int { return (caller->*func)(arg); }, argument);
    }

    Future ThreadPool::enqueue(Function&& function, void* argument)
    {
        Task task(function);
        Future future = task.get_future();
        _queue.enqueue(_token, new TaskWrapper(task, argument));

        return future;
    }

}
