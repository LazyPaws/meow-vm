#include "meow-vm/meow_vm.h"

MeowVM::MeowVM(const std::string& entry, int argc, char** argv) : entryPointDir(entry) {
    commandLineArgs.reserve(argc);

    for (int i = 0; i < argc; ++i) {
        commandLineArgs.push_back(argv[i]);
    }
}

void MeowVM::interpret(const std::string& entryPath) {
    states.reset();

    try {

    } catch (const std::exception& e) {

    } catch (...) {
        
    }
}