#include "meow-vm/meow_vm.h"

using namespace meow::vm;

MeowVM::MeowVM(const std::string& entry, int argc, char** argv) : entryPointDir(entry) {
    commandLineArgs.reserve(argc);

    for (int i = 0; i < argc; ++i) {
        commandLineArgs.push_back(argv[i]);
    }
}

void MeowVM::interpret(const std::string& entryPath) {
    state.reset();

    try {

    } catch (const std::exception& e) {

    } catch (...) {
        
    }
}