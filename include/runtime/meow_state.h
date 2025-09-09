#pragma once

#include "common/value.h"
#include "common/definitions.h"
#include "common/pch.h"

namespace meow::memory {
    struct MeowObject;
}

namespace meow::runtime {
    struct MeowState {
        std::vector<meow::common::CallFrame> callStack;
        std::vector<meow::common::Value> stackSlots;
        std::vector<meow::common::Upvalue> openUpvalues;
        std::unordered_map<std::string, meow::common::Module> moduleCache;
        std::vector<meow::common::ExceptionHandler> exceptionHandlers;
        
        void reset() {
            callStack.clear();
            stackSlots.clear();
            openUpvalues.clear();
            moduleCache.clear();
            exceptionHandlers.clear();
        }

        inline std::vector<meow::memory::MeowObject*> getRoots() const {

        }
    };
}