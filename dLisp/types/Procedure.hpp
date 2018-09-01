/*! 
 * \file
 * \author Павел Коваленко
 * \date 23 июля 2018 г., 22:32
 */

#ifndef PROCEDURE_T_HPP
#define PROCEDURE_T_HPP

#include "../mm_ptr.hpp"
#include <vector>
#include <functional>

using Function = obj_ptr(*)(obj_ptr, env_ptr);

inline obj_ptr undefined() {
    return makeObject(T_SPECIAL, Special(UNDEF));
}

/*!
 * \brief Класс lisp-типа процедуры
 * 
 * Данный класс предназначен для представления lisp-процедур, лямбда-фйнкций и
 * замыканий
 */
struct Procedure {
    Function function = nullptr;
    size_t minArgsc = 0;
    size_t maxArgsc = 0;
    std::vector<obj_ptr>* formalArgs = nullptr;
    obj_ptr body;
    env_ptr environment;
    
    Procedure() {}
    Procedure(Function f, size_t min, size_t max) : function(f), minArgsc(min), maxArgsc(max) {}
    Procedure(const Procedure& p);
    Procedure& operator = (const Procedure&);
    bool operator == (const Procedure&);
    obj_ptr apply(obj_ptr, env_ptr&);
    static void checkArgsCount(obj_ptr proc, obj_ptr args);
};

#endif /* PROCEDURE_T_HPP */

