/*! 
 * \file
 * \author Павел Коваленко
 * \date 27 июля 2018 г., 22:00
 */

#ifndef PAIR_T_HPP
#define PAIR_T_HPP

#include "../mm_ptr.hpp"

/*!
 * \brief Класс lisp-типа пар
 * 
 * Данный класс используется для представления пар и списков. 
 */
struct pair_t {
    obj_ptr car;
    obj_ptr cdr;
    pair_t(obj_ptr head, obj_ptr tail) : car(head), cdr(tail) {}
    inline bool operator == (const pair_t& other) {
        return (car == other.car) and (cdr == other.cdr);
    }
};

#endif /* PAIR_T_HPP */

