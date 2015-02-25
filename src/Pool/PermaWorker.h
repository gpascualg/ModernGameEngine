#pragma once

#include "Pool.h"
#include "WorkerThread.h"

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
