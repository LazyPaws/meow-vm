#pragma once

#include "common/pch.h"

namespace meow::runtime::module {
    struct Module;
    struct Diagnostic;

    struct ModuleManager {
        
        Module loadModule(const std::string& modulePath);
    };
}