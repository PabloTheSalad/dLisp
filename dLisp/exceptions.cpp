#include "exceptions.hpp"
#include "repl.hpp"
#include "lispTypes.hpp"


//! Вызывает ошибку если p не true
void assert(bool p, const char* err_msg, const obj_ptr& obj) {
    if (!p) {
        if (!obj.isValid()) throw LispException(err_msg);
        else throw evalError(err_msg, obj);
    }
}

//! Используется для проверки правильности синтаксиса
void assertSyntax(bool p, const char* op, const obj_ptr& obj) {
    if (!p) {
        throw syntaxError(op, obj);
    }
}

//! Используется для проверки правильности типов списка
void assertArgsType(LispTypeFlag type, obj_ptr list) {
    if (!list->isList()) return;
    for (size_t i = 1; list->type != T_EMPTY; list = list->pair().cdr, i++) {
        assertArgType(type, list->pair().car, i);
    }
}

//! Используется для проверки правльности типа объекта
void assertArgType(LispTypeFlag type, obj_ptr arg, size_t pos) {
    if (arg->type != type) {
        LispException err("Wrong type argument in position ", arg);
        err.errorString += std::to_string(pos) + ": " + objectAsString(arg);
        err.addProc = true;
        throw err;
    }
}

//! Возвращает объект исключения поднимаемого при парсинге
LispException parseError(const char* str) {
    std::string err_str = "Parse error: ";
    err_str += str;
    return LispException(std::move(err_str));
}

//! Возвращает объект исключения поднимаемого при лексическом разборе
LispException lexicalError(const char* str) {
    std::string err_str = "lexical error: unexpected \"";
    err_str += str;
    err_str += '"';
    return LispException(std::move(err_str));
}

//! Возвращает объект исключения поднимаемого при ошибке при вычислении выражения
LispException evalError(const char* str, obj_ptr obj) {
    std::string err_str = "Eval error: ";
    err_str += str;
    err_str += " [ error object: " + objectAsString(obj) + " ]";
    return LispException(std::move(err_str), obj);
}

//! Возвращает объект исключения поднимаемого при несоответсвии синтаксиса
LispException syntaxError(const char* op, obj_ptr obj) {
    std::string err_str = "Syntax error: ";
    err_str += "bad ";
    err_str += op;
    err_str += " syntax in form: " + objectAsString(obj);
    return LispException(std::move(err_str), obj);
}

//! Возвращает объект исключения поднимаемого при ошибке доступа к памяти
LispException memoryError(const char* str) {
    std::string err_str = "Memory error: ";
    err_str += str;
    LispException err = LispException(std::move(err_str));
    err.isCritical = true;
    return err;
}
