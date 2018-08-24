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

#include "lisp_type_flag.hpp"
#include "mm_ptr.hpp"


struct lisp_error : public std::exception {
    std::string err_str;
    obj_ptr err_obj;
    bool add_proc = false;
    bool critical = false;
    lisp_error(std::string&& str) : err_str(str) {}
    lisp_error(std::string&& str, obj_ptr obj) : err_str(str), err_obj(obj) {}
    const char* what() const noexcept { return err_str.c_str(); } 
};

lisp_error parse_error(const char* str);
lisp_error eval_error(const char* str, obj_ptr obj);
lisp_error syntax_error(const char* op, obj_ptr obj);
lisp_error memory_error(const char*);

void assert(bool, const char*, const obj_ptr& obj = obj_ptr());
void assert_syntax(bool p, const char* op, const obj_ptr& obj);
void assert_args_type(lisp_type_flag type, obj_ptr list);

#endif /* EXCEPTIONS_HPP */

