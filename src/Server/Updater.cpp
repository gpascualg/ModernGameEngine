#include "Updater.h"

#include "ThreadPool.h"

namespace Core {

	Pool::Future Updater::addToPool()
	{
		return Pool::ThreadPool::get()->enqueue(Pool::Function(entry_point), this);
	}

}