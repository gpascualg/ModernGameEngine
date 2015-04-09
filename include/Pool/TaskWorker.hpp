#pragma once

#include "Pool.hpp"
#include "WorkerThread.hpp"

namespace Pool {

    class TaskWorker : public WorkerThread {
        friend ThreadPool;

    private:
        TaskWorker(Queue* queue, int dequeueCount);

        virtual void _functor() override;
    };

}
