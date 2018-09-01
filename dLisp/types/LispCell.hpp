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
    LispCell (const LispCell&) = default;
    LispCell& operator = (const LispCell&) = default;
    LispCell (LispCell&&) = default;
    LispCell& operator = (LispCell&&) = default;
    ~LispCell () {}
    LispCell (LispTypeFlag type) : type(type) {}
    template <class T> LispCell (LispTypeFlag type, const T&& value);
    bool operator == (const LispCell&);
    inline bool operator != (const LispCell& other) { return !(*this == other); }
    bool operator < (const LispCell&);

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
LispCell::LispCell (LispTypeFlag type, const T&& value) : type(type) {
    if constexpr (std::is_same<T, Number>::value) {
        number = value;
    } else if constexpr (std::is_same<T, Bool>::value) {
        boolean = value;
    } else if constexpr (std::is_same<T, Special>::value) {
        spec = value;
    }
}

bool checkListType(LispTypeFlag, obj_ptr);
obj_ptr singletonList(obj_ptr);
obj_ptr emptyList();

#endif /* LISP_T_HPP */
