#pragma once

#include "Pool.h"

namespace Pool {

    struct TaskWrapper
    {
        Task task;
        void* argument;

        TaskWrapper();
        TaskWrapper(Task& _task, void* _argument);
    };

}
