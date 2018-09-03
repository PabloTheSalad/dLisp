/* 
 * File:   main.cpp
 * Author: pablo
 *
 * Created on 19 июля 2018 г., 21:23
 */

#include <memory>
#include "repl.hpp"
#include "lispTypes.hpp"


int main(int argc, char* argv[]) {
    std::unique_ptr<MemoryManager> memoryManager(initializeMemoryManager());
    auto globalEnvironment = makeGlobalEnv();
    evalFile("funcs.dlisp", globalEnvironment);
    if (argc == 1) {
        repl(memoryManager.get(), globalEnvironment);
    } else {
        for (int i = 1; i < argc; i++) {
            evalFile(argv[i], globalEnvironment);
        }
    }
    return 0;
}
