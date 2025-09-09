#pragma once

#include "common/pch.h"
#include "memory/garbage_collector.h"
#include "runtime/meow_state.h"

namespace meow::memory {
    struct MemoryManager {
    private:
        std::unique_ptr<GarbageCollector> gc;
        size_t allocated;
        size_t threshold;

        meow::runtime::MeowState* state;
    public:
        MemoryManager(std::unique_ptr<GarbageCollector> garbageCollector);
        template <typename T, typename ... Args>
        T* newObject(Args&& ... args) {
            if (allocated >= threshold) {
                gc->collect()
            }
            T* newObject = new T(std::forward<Args>(args)...);
            gc->registerObject(static_cast<MeowObject*>(newObject));
            ++allocated;
            return newObject;
        }

        inline void collect() {
            if (!state) return;
            gc->collect(*state);
            allocated = 0;
        }

        inline void setState(meow::runtime::MeowState* meowState) noexcept {
            state = meowState;
        }
    };
}