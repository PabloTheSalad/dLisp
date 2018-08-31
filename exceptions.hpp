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


struct LispException : public std::exception {
    std::string errorString;
    obj_ptr errorObject;
    bool addProc = false;
    bool isCritical = false;
    LispException(std::string&& str) : errorString(str) {}
    LispException(std::string&& str, obj_ptr obj) : errorString(str), errorObject(obj) {}
    virtual ~LispException() {}
    virtual const char* what() const noexcept { return errorString.c_str(); }
};

LispException parseError(const char* str);
LispException evalError(const char* str, obj_ptr obj);
LispException syntaxError(const char* op, obj_ptr obj);
LispException memoryError(const char*);

void assert(bool, const char*, const obj_ptr& obj = obj_ptr());
void assertSyntax(bool p, const char* op, const obj_ptr& obj);
void assertArgsType(LispTypeFlag type, obj_ptr list);

#endif /* EXCEPTIONS_HPP */

