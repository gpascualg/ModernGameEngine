#pragma once

#include "core.h"
#include "Pool.h"

namespace Pool {

    class WorkerThread {
        friend ThreadPool;

    protected:

        virtual void _functor() = 0;

        static void _static_functor(WorkerThread* me) {
            me->_functor();
        }

        WorkerThread(Queue* queue, int bulkDequeue):
            _queue(queue),
            _bulkDequeue{bulkDequeue},
            _busy{false},
            _stop{false}
        {
        }

        WorkerThread(nullptr_t n):
            _busy{false},
            _stop{false}
        {
        }

        LFS_INLINE void start()
        {
            _thread = std::move(std::thread(_static_functor, this));
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

}
