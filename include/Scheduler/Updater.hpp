#pragma once

#include <atomic>

#include "concurrentqueue.hpp"
#include "Pool.hpp"

namespace Core
{
    class Updater;

    template <typename T>
    class Scheduler;

    class Updater
    {
        template <typename T>
        friend class Scheduler;

    public:
        virtual ~Updater();

    protected:
        Updater();

		virtual int updateCPU(void* arg0);
		virtual int updateGPU();

    protected:
        int _bulkDequeue;
    };
}
