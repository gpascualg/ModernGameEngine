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
        virtual ~Updater()
        {
        }

    protected:
        Updater()
        {
        }

        Pool::Future addToPool();

        virtual int update()
        {
            return 0;
        }

    private:
        static int entry_point(void* me)
        {
            return static_cast<Updater*>(me)->update();
        }

    protected:
        int _bulkDequeue;
    };
}
