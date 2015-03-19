#include "Updater.hpp"

#include "ThreadPool.hpp"

namespace Core {

    Pool::Future Updater::addToPool()
    {
        return Pool::ThreadPool::get()->enqueue(Pool::Function(entry_point), this);
    }

}
