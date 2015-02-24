#include <atomic>

#include "concurrentqueue.h"

namespace Core
{
	typedef std::function<> Function;
	typedef moodycamel::ConcurrentQueue<Function> Queue;
	typedef moodycamel::ProducerToken ProducerToken;
	typedef moodycamel::ProducerToken ConsumerToken;

	class Updater
	{
	protected:
		Updater(Queue* queue):
			_queue(queue),
			_producerToken(*queue),
			_consumerToken(*queue)
		{

		}

	protected:
		Queue* _queue;
		ProducerToken _producerToken;
		ConsumerToken _consumerToken;
	};
}

