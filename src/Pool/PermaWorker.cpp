#include "PermaWorker.h"

namespace Pool {

	PermaWorker::PermaWorker(Function& function, void* argument):
		WorkerThread(nullptr),
		_function(std::move(function)),
		_argument(argument)
	{
	}

	void PermaWorker::_functor()
    {
		while (!_stop)
		{
            _busy = true;
				_function(_argument);
			_busy = false;
		}
	}

}
