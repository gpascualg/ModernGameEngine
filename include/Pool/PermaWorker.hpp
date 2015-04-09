#pragma once

#include "Pool.hpp"
#include "WorkerThread.hpp"

namespace Pool {

    class PermaWorker : public WorkerThread {
        friend ThreadPool;


    private:
        PermaWorker(Function& function, void* argument);

        virtual void _functor() override;

        Function _function;
        void* _argument;
    };

}
