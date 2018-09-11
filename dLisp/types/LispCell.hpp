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


/*!
 * \brief Класс представляющий lisp-ячейку и позволящий
 * хранить значения различных lisp-типов
 */
struct LispCell {
    using obj_ptr = mm_ptr<LispCell>;

    LispTypeFlag type = T_EMPTY;
    int refCounter = 0;
    bool isMutable = false;
    void* value = nullptr;

    template<class T>
    inline T* getValue() const {
        return static_cast<T*>(value);
    }
    Number& number() const { return *getValue<Number>(); }
    String& string() const { return *getValue<String>(); }
    Symbol& symbol() const { return *getValue<Symbol>(); }
    Procedure& procedure() const { return *getValue<Procedure>(); }
    Pair& pair() const { return *getValue<Pair>(); }
    Bool& boolean() const { return *getValue<Bool>(); }
    Special& special() const { return *getValue<Special>(); }
    Environment& environment() const { return *getValue<Environment>(); }

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
                return number() < rhs.number();
            case T_STRING:
            case T_SYMBOL:
                return string() < rhs.string();
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
    bool isEmpty();
    bool isList();
    bool isPairList();
    bool isPairSyntax();
};

using obj_ptr = mm_ptr<LispCell>;

template <class T>
LispCell::LispCell (LispTypeFlag type, T&& val)
    : type(type), isMutable(false) {
    if constexpr (std::is_same<T, Number>::value
                  || std::is_same<T, Bool>::value
                  || std::is_same<T, Special>::value) {
        value = new T(std::forward<T>(val));
    }
}

bool checkListType(LispTypeFlag, obj_ptr);
obj_ptr singletonList(obj_ptr);
obj_ptr emptyList();

#endif /* LISP_T_HPP */
