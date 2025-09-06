#pragma once

namespace meow::memory {
    struct MeowObject;

    struct GCVisitor {
        virtual ~GCVisitor() = default;
        virtual void visitValue(meow::common::Value&) = 0;
        virtual void visitObject(MeowObject*) = 0;
    };
}