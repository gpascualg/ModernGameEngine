#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "counter.h"
#include "pool.h"

int main() {
	Counter c;

	Pool::ThreadPool* threadPool = Pool::ThreadPool::get(10);
	Pool::Future result = threadPool->enqueue([&c](void*) -> int { 
		printf("%lu\n", c.inc()); 
		return 1; 
	}, nullptr);
	getchar();

	threadPool->stop();
	threadPool->join();

	printf("RESULT: %d\n", result.get());

	getchar();

	delete threadPool;

	return 0;
}
