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

struct LispCell;
using obj_ptr = mm_ptr<LispCell>;

class ListIterator {
    LispCell* cell;
public:
    ListIterator(LispCell* c) : cell(c) {}
    ListIterator operator ++();
    bool operator !=(const ListIterator& other) { return cell != other.cell; }
    bool operator ==(const ListIterator& other) { return cell == other.cell; }
    obj_ptr& operator * ();
    LispCell* operator -> ();
    LispCell* get() { return cell; }
};

namespace std {
template<>
struct iterator_traits<ListIterator> {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = size_t;
    using value_type = LispCell;
    using pointer = LispCell*;
    using refernece = LispCell&;
};
}


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
    inline T* get() const {
        return static_cast<T*>(value);
    }
    Number& number() const { return *get<Number>(); }
    String& string() const { return *get<String>(); }
    Symbol& symbol() const { return *get<Symbol>(); }
    Procedure& procedure() const { return *get<Procedure>(); }
    Pair& pair() const { return *get<Pair>(); }
    Bool& boolean() const { return *get<Bool>(); }
    Special& special() const { return *get<Special>(); }
    Environment& environment() const { return *get<Environment>(); }

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
    ListIterator begin();
    ListIterator end();
    
    bool isAtom();
    bool isSelfEvaluating();
    bool isTrue();
    bool isEmpty();
    bool isList();
    bool isPairList();
    bool isPairSyntax();
};

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
