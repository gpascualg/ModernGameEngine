#pragma once

#include "Pool.hpp"

namespace Pool {

    struct TaskWrapper
    {
        Task task;
        void* argument;

        TaskWrapper();
        TaskWrapper(Task& _task, void* _argument);
    };

}
