/* 
 * File:   lisp_t.hpp
 * Author: pablo
 *
 * Created on 23 июля 2018 г., 21:52
 */

#ifndef LISP_T_HPP
#define LISP_T_HPP

#include "LispTypeFlag.hpp"

#include "types/Number.hpp"
#include "types/String.hpp"
struct Procedure;
class Environment;
struct Pair;
#include "types/Bool.hpp"
#include "types/Special.hpp"

template<class T> class mm_ptr;

struct LispCell {
    using obj_ptr = mm_ptr<LispCell>;

    LispTypeFlag type = T_EMPTY;
    int refCounter = 0;
    bool isMutable = false;
    union {
        Number number;
        String* string;
        Symbol* symbol;
        Procedure* proc;
        Pair* pair;
        Bool boolean;
        Special spec;
        Environment* env;
    };


    LispCell () {}
    LispCell (const LispCell&) = delete;
    LispCell& operator = (LispCell&) = delete;
    LispCell (LispCell&&) = default;
    LispCell& operator = (LispCell&&) = default;
    LispCell (LispTypeFlag type) : type(type) {}
    template <class T> LispCell (LispTypeFlag type, T&& value);
    ~LispCell () {}

    bool operator == (const LispCell&);
    inline bool operator != (const LispCell& other) { return !(*this == other); }
    inline bool operator < (const LispCell& rhs) const {
        if (type != rhs.type) return type < rhs.type;
        switch(type) {
            case T_NUMBER:
                return number < rhs.number;
            case T_STRING:
            case T_SYMBOL:
                return *string < *rhs.string;
            default:
                return false;
        }
    }

    void clear();
    void append(obj_ptr);
    size_t len();
    obj_ptr at(int);
    obj_ptr end();
    
    bool isAtom();
    bool isSelfEvaluating();
    bool isTrue();
    bool isNull();
    bool isList();
    bool isPair();
    bool isPairSyntax();
};

using obj_ptr = mm_ptr<LispCell>;

template <class T>
LispCell::LispCell (LispTypeFlag type, T&& value) : type(type) {
    static_assert(!std::is_const<T>::value, "Bad type for LispCell constructor");
    if constexpr (std::is_same<T, Number>::value) {
        number = std::move(value);
    } else if constexpr (std::is_same<T, Bool>::value) {
        boolean = std::move(value);
    } else if constexpr (std::is_same<T, Special>::value) {
        spec = std::move(value);
    }
}

bool checkListType(LispTypeFlag, obj_ptr);
obj_ptr singletonList(obj_ptr);
obj_ptr emptyList();

#endif /* LISP_T_HPP */
