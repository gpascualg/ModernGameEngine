#pragma once

#include <atomic>
#include <cmath>
#include <future>
#include <functional>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <vector>


#include "core.h"
#include "concurrentqueue.h"


namespace Pool
{
	class ThreadPool;
	class WorkerThread;

	class TaskWorker;
	class PermaWorker;

	struct TaskWrapper;

	using Queue = moodycamel::ConcurrentQueue<TaskWrapper*>;
	using ConsumerToken = moodycamel::ConsumerToken;
	using ProducerToken = moodycamel::ProducerToken;
	using Function = std::function<int(void*)>;
	using Task = std::packaged_task<int(void*)>;
	using Future = std::future<int>;
	
};
