#pragma once

namespace meow::memory {
    struct GCVisitor;

    struct MeowObject {
        virtual ~MeowObject() = default;
        virtual void trace(GCVisitor&) = 0;
    };
};