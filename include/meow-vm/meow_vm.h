#pragma once

#include "runtime/meow_state.h"

namespace meow::vm {
    class MeowVM {
    public:
        MeowVM(const std::string& entry, int argc, char** argv);
        void interpret(const std::string& entryPath);
    private:
        std::string entryPointDir;
        std::vector<std::string> commandLineArgs;
        meow::runtime::MeowState state;
    };
}
