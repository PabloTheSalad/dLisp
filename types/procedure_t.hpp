/*! 
 * \file
 * \author Павел Коваленко
 * \date 23 июля 2018 г., 22:32
 */

#ifndef PROCEDURE_T_HPP
#define PROCEDURE_T_HPP

#include "../mm_ptr.hpp"
#include <vector>

typedef obj_ptr(*function_t)(obj_ptr, env_ptr);

/*!
 * \brief Класс lisp-типа процедуры
 * 
 * Данный класс предназначен для представления lisp-процедур, лямбда-фйнкций и
 * замыканий
 */
struct procedure_t {
    function_t function = nullptr;
    size_t argsc = 0;
    obj_ptr body;
    std::vector<obj_ptr>* formal_args = nullptr;
    env_ptr environment;
    
    procedure_t() {}
    procedure_t(function_t f, size_t c) : function(f), argsc(c) {}
    procedure_t(const procedure_t& p);
    procedure_t& operator = (const procedure_t&);
    bool operator == (const procedure_t&);
    obj_ptr apply(obj_ptr, env_ptr&);
};

#endif /* PROCEDURE_T_HPP */

