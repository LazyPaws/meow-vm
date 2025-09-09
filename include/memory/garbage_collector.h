#pragma once

namespace meow::runtime { 
    struct MeowState;
}

namespace meow::memory {
    struct MeowState;
    struct MeowObject;

    class GarbageCollector {
    public:
        virtual ~GarbageCollector() = default;
        
        virtual void registerObject(MeowObject*) = 0;
        
        virtual void collect(meow::runtime::MeowState&) = 0;
    };
}