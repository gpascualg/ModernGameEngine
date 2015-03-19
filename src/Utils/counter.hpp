#pragma once

#include <atomic>
#include "core.hpp"

class Counter {
public:
    Counter() LFS_NOEXCEPT :
        _counter{0}
    {}

    LFS_INLINE uint64_t inc() {
        return _counter.fetch_add(1, std::memory_order_relaxed);
    }

    std::atomic<uint64_t> _counter;
};
