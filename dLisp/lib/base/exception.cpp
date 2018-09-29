#include "exception.hpp"
#include "exceptions.hpp"
#include "tools.hpp"
#include "repl.hpp"

FuncTable Base::exceptionFuncTable() {
    return {
        {"error", v(error, 1, 0)}
    };
}

obj_ptr error(obj_ptr args) {
    LispException err("");
    err.addProc = true;
    for (auto& cell : *args) {
        if (cell->type == T_STRING)
            err.errorString += cell->string();
        else
            err.errorString += objectAsString(cell);
        err.errorString += " ";
    }
    throw err;
}
