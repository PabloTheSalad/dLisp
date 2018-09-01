#include "exception.hpp"
#include "exceptions.hpp"
#include "tools.hpp"
#include "repl.hpp"

obj_ptr error(obj_ptr args) {
    LispException err("");
    err.addProc = true;
    forAllInList(args, [&err](auto obj){
        err.errorString += objectAsString(obj) + " ";
    });
    throw err;
}