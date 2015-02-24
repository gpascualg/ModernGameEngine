#pragma once

#include <atomic>
#include "core.h"

class Counter {
public:
	Counter() noexcept :
		_counter{0}
	{}

	LFS_INLINE uint64_t inc() {
		return _counter.fetch_add(1, std::memory_order_relaxed);
	}

	std::atomic<uint64_t> _counter;
};
