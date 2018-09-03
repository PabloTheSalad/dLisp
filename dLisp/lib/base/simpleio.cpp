#include "simpleio.hpp"
#include "repl.hpp"
#include "tools.hpp"

#include <iostream>

FuncTable Base::simpleIoFuncTable() {
    return {
        {"display", c(display, 1)},
        {"write", c(write, 1)},
        {"newline", c(newLine, 0)},
        {"print", v(print, 0, SIZE_MAX)}
    };
}

obj_ptr display(obj_ptr args) {
    obj_ptr obj = args->at(0);
    if (obj->type != T_STRING) {
        std::cout << objectAsString(obj);
    } else {
        std::cout << *obj->string;
    }
    return unspecified();
}

obj_ptr write(obj_ptr args) {
    obj_ptr obj = args->at(0);
    std::cout << objectAsString(obj);
    return unspecified();
}

obj_ptr newLine(obj_ptr) {
    std::cout << std::endl;
    return unspecified();
}

obj_ptr print(obj_ptr args) {
    forAllInList(args, [](auto obj){
        if (obj->type != T_STRING) {
            std::cout << objectAsString(obj);
        } else {
            std::cout << *obj->string;
        }
    });
    std::cout << std::endl;
    return unspecified();
}
