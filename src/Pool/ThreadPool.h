#pragma once

#include "Pool.h"

namespace Pool {

    class ThreadPool {
        friend class WorkerThread;
        friend class TaskWorker;

    private:
        ThreadPool(int nThreads, int bulkDequeue);

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

        ~ThreadPool();

        Future enqueue(Function&& function, void* argument);
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

}