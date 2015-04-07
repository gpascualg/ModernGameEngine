#include "WorkerThread.hpp"

namespace Pool {

    WorkerThread::WorkerThread(Queue* queue, int bulkDequeue):
        _queue(queue),
        _bulkDequeue{bulkDequeue},
        _busy{false},
        _stop{false}
    {
    }

    WorkerThread::WorkerThread(nullptr_t n):
        _busy{false},
        _stop{false}
    {
    }

    void WorkerThread::_static_functor(WorkerThread* me) {
        me->_functor();
    }
}
