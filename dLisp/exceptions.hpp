/* 
 * File:   exceptions.hpp
 * Author: pablo
 *
 * Created on 26 июля 2018 г., 1:00
 */

#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <cstdio>
#include <cstdlib>
#include <stdexcept>

#include "LispTypeFlag.hpp"
#include "mm_ptr.hpp"

/*!
 * \brief Структура представляеющая внутренее исключение интерпретатора
 *
 * Данное исключение используется для внутрених ошибок интерпретатора,
 * и только оно может быть поймано в REPL-режиме
 */
struct LispException : public std::exception {
    std::string errorString; //! Строка описания ошибки
    obj_ptr errorObject; //! Объект в котором или из-за которого произошла ошибка
    bool addProc = false; //! Надо ли добавлять к описанию ошибки в какой процедуре она произошла
    bool isCritical = false; //! Является ли ошибка критической
    LispException(std::string&& str) : errorString(str) {}
    LispException(std::string&& str, obj_ptr obj) : errorString(str), errorObject(obj) {}
    virtual ~LispException() {}
    virtual const char* what() const noexcept { return errorString.c_str(); }
};

LispException parseError(const char* str);
LispException lexicalError(const char* str);
LispException evalError(const char* str, obj_ptr obj);
LispException syntaxError(const char* op, obj_ptr obj);
LispException memoryError(const char*);

void assert(bool, const char*, const obj_ptr& obj = obj_ptr());
void assertSyntax(bool p, const char* op, const obj_ptr& obj);
void assertArgType(LispTypeFlag type, obj_ptr arg, size_t pos);
void assertArgsType(LispTypeFlag type, obj_ptr list);

#endif /* EXCEPTIONS_HPP */

