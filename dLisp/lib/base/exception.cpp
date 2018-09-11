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
    forAllInList(args, [&err](auto obj){
        if (obj->type == T_STRING)
            err.errorString += obj->string();
        else
            err.errorString += objectAsString(obj);
        err.errorString += " ";
    });
    throw err;
}
