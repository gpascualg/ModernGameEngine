#pragma once

#include "Pool.h"
#include "WorkerThread.h"

namespace Pool {

	class TaskWorker : public WorkerThread {
		friend ThreadPool;

    private:
		TaskWorker(Queue* queue, int dequeueCount);

        virtual void _functor() override;
	};

}
