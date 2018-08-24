/* 
 * File:   lisp_t.hpp
 * Author: pablo
 *
 * Created on 23 июля 2018 г., 21:52
 */

#ifndef LISP_T_HPP
#define LISP_T_HPP

#include "lisp_type_flag.hpp"

#include "types/number_t.hpp"
#include "types/string_t.hpp"
struct procedure_t;
class environment_t;
struct pair_t;
#include "types/bool_t.hpp"
#include "types/special_t.hpp"
template<class T> class mm_ptr;

struct lisp_t {
    using obj_ptr = mm_ptr<lisp_t>;

    lisp_type_flag type;
    int ref_counter = 0;
    union {
        number_t number;
        string_t* string_ptr;
        symbol_t* symbol_ptr;
        procedure_t* proc;
        pair_t* pair;
        bool_t boolean;
        special_t spec;
        environment_t* env;
    };
    lisp_t () {}
    lisp_t (const lisp_t&);
    ~lisp_t () {}
    lisp_t (lisp_type_flag type) : type(type) {}
    template <class T> lisp_t (lisp_type_flag type, const T& value);
    bool operator == (const lisp_t&);
    inline bool operator != (const lisp_t& other) { return !(*this == other); }
    bool operator < (const lisp_t&);
    lisp_t& operator = (const lisp_t&);
    void append(obj_ptr);
    size_t len();
    obj_ptr at(int);
    obj_ptr end();
    
    bool is_atom();
    bool is_self_evaluating();
    bool is_true();
    bool is_null();
    bool is_list();
    bool is_pair();
    bool is_pair_syntax();
};

using obj_ptr = mm_ptr<lisp_t>;

template <class T>
lisp_t::lisp_t (lisp_type_flag type, const T& value) : type(type) {
    if constexpr (std::is_same<T, number_t>::value) {
        number = value;
    } else if constexpr (std::is_same<T, bool_t>::value) {
        boolean = value;
    } else if constexpr (std::is_same<T, special_t>::value) {
        spec = value;
    }
}

bool check_list_type(lisp_type_flag, obj_ptr);
obj_ptr singleton_list(obj_ptr);
obj_ptr empty_list();

#endif /* LISP_T_HPP */
