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

        virtual int update();

    private:
        static int entry_point(void* me);

    protected:
        int _bulkDequeue;
    };
}
