#pragma once

#include "core.hpp"
#include "Pool.hpp"

namespace Pool {

    class WorkerThread {
        friend ThreadPool;

    protected:

        WorkerThread(Queue* queue, int bulkDequeue);
        WorkerThread(nullptr_t n);

        static void _static_functor(WorkerThread* me);
        virtual void _functor() = 0;

        LFS_INLINE void start();
        LFS_INLINE std::thread::id getID();

    protected:
        Queue* _queue;

        std::thread _thread;
        int _bulkDequeue;
        std::atomic<bool> _busy;
        std::atomic<bool> _stop;
    };


    // INLINE METHODS

    void WorkerThread::start()
    {
        _thread = std::move(std::thread(_static_functor, this));
    }

    std::thread::id WorkerThread::getID()
    {
        return std::this_thread::get_id();
    }
}
