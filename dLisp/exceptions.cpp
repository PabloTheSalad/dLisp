#include "exceptions.hpp"
#include "repl.hpp"
#include "lispTypes.hpp"

void assert(bool p, const char* err_msg, const obj_ptr& obj) {
    if (!p) {
        if (obj.isNull()) throw LispException(err_msg);
        else throw evalError(err_msg, obj);
    }
}

void assertSyntax(bool p, const char* op, const obj_ptr& obj) {
    if (!p) {
        throw syntaxError(op, obj);
    }
}

void assertArgsType(LispTypeFlag type, obj_ptr list) {
    if (!list->isList()) return;
    for (int i = 1; list->type != T_EMPTY; list = list->pair->cdr, i++) {
        if (list->pair->car->type != type) {
            LispException err("Wrong type argument in position ", list->pair->car);
            err.errorString += std::to_string(i) + ": " + objectAsString(list->pair->car);
            err.addProc = true;
            throw err;
        }
    }
}

LispException parseError(const char* str) {
    std::string err_str = "Parse error: ";
    err_str += str;
    return LispException(std::move(err_str));
}

LispException evalError(const char* str, obj_ptr obj) {
    std::string err_str = "Eval error: ";
    err_str += str;
    err_str += " [ error object: " + objectAsString(obj) + " ]";
    return LispException(std::move(err_str), obj);
}

LispException syntaxError(const char* op, obj_ptr obj) {
    std::string err_str = "Syntax error: ";
    err_str += "bad ";
    err_str += op;
    err_str += " syntax in form: " + objectAsString(obj);
    return LispException(std::move(err_str), obj);
}

LispException memoryError(const char* str) {
    std::string err_str = "Memory error: ";
    err_str += str;
    LispException err = LispException(std::move(err_str));
    err.isCritical = true;
    return err;
}
