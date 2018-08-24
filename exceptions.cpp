#include "exceptions.hpp"
#include "repl.hpp"
#include "lisp_types.hpp"

void assert(bool p, const char* err_msg, const obj_ptr& obj) {
    if (!p) {
        if (obj.is_null()) throw lisp_error(err_msg);
        else throw eval_error(err_msg, obj);
    }
}

void assert_syntax(bool p, const char* op, const obj_ptr& obj) {
    if (!p) {
        throw syntax_error(op, obj);
    }
}

void assert_args_type(lisp_type_flag type, obj_ptr list) {
    if (!list->is_list()) return;
    for (int i = 1; list->type != T_NULL; list = list->pair->cdr, i++) {
        if (list->pair->car->type != type) {
            lisp_error err("Wrong type argument in position ", list->pair->car);
            err.err_str += std::to_string(i) + ": " + obj_as_str(list->pair->car);
            err.add_proc = true;
            throw err;
        }
    }
}

lisp_error parse_error(const char* str) {
    std::string err_str = "Parse error: ";
    err_str += str;
    return lisp_error(std::move(err_str));
}

lisp_error eval_error(const char* str, obj_ptr obj) {
    std::string err_str = "Eval error: ";
    err_str += str;
    err_str += " [ error object: " + obj_as_str(obj) + " ]";
    return lisp_error(std::move(err_str), obj);
}

lisp_error syntax_error(const char* op, obj_ptr obj) {
    std::string err_str = "Syntax error: ";
    err_str += "bad ";
    err_str += op;
    err_str += " syntax in form: " + obj_as_str(obj);
    return lisp_error(std::move(err_str), obj);
}

lisp_error memory_error(const char* str) {
    std::string err_str = "Memory error: ";
    err_str += str;
    lisp_error err = lisp_error(std::move(err_str));
    err.critical = true;
    return err;
}
