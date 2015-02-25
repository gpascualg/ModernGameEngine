#include "TaskWrapper.h"

namespace Pool {
	
	TaskWrapper::TaskWrapper() {}

	TaskWrapper::TaskWrapper(Task& _task, void* _argument):
		task(std::move(_task)),
		argument(_argument)
	{
	}

}