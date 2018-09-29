#include "simpleio.hpp"
#include "repl.hpp"
#include "tools.hpp"

#include <iostream>

FuncTable Base::simpleIoFuncTable() {
    return {
        {"display", c(display, 1)},
        {"write", c(write, 1)},
        {"newline", c(newLine, 0)},
        {"print", v(print, 0, SIZE_MAX)},
        {"read", c(read, 0)},
        {"read-line", c(readLine, 0)}
    };
}

obj_ptr display(obj_ptr args) {
    obj_ptr obj = args->at(0);
    if (obj->type != T_STRING) {
        std::cout << obj;
    } else {
        std::cout << obj->string();
    }
    return unspecified();
}

obj_ptr write(obj_ptr args) {
    obj_ptr obj = args->at(0);
    std::cout << obj;
    return unspecified();
}

obj_ptr newLine(obj_ptr) {
    std::cout << std::endl;
    return unspecified();
}

obj_ptr print(obj_ptr args) {
    for (auto& cell : *args) {
        if (cell->type != T_STRING) {
            std::cout << cell;
        } else {
            std::cout << cell->string();
        }
    }
    std::cout << std::endl;
    return unspecified();
}

obj_ptr read(obj_ptr) {
    obj_ptr form;
    while (true) {
    form = tokenizeAndParseForm(std::cin);
    if (form.isValid()) break;
    }
    return form;
}

obj_ptr readLine(obj_ptr) {
    char line[256];
    if (std::cin.peek() == '\n') {
        std::cin.get();
        std::cin.getline(line, 265);
        std::cin.putback('\n');
    } else std::cin.getline(line, 265);
    return makeString(line);
}
