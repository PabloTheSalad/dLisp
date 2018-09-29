/* 
 * File:   main.cpp
 * Author: pablo
 *
 * Created on 19 июля 2018 г., 21:23
 */

#include <memory>
#include "repl.hpp"
#include "lispTypes.hpp"
#include "exceptions.hpp"


int main(int argc, char* argv[]) {
    std::unique_ptr<MemoryManager> memoryManager(initializeMemoryManager());
    auto globalEnvironment = makeGlobalEnv();
    evalFile("funcs.dlisp", globalEnvironment);
    if (argc == 1) {
        repl(memoryManager.get(), globalEnvironment);
    } else {
        int i = 1;
        try {
            for (; i < argc; i++) {
                if(!evalFile(argv[i], globalEnvironment)) {
                    throw LispException("File not found");
                }
            }
        } catch (const LispException& e) {
            std::cout << "Fatal error: In file "
                      << argv[i] << ": " << e.what() << std::endl;
            return e.what()[0];
        }
    }
    return 0;
}
