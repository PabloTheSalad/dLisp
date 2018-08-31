#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <functional>

#include "mm_ptr.hpp"
#include "lispTypes.hpp"

void forAllInList(obj_ptr obj, std::function<void(obj_ptr&)> func);

obj_ptr copyObject(obj_ptr);

//! Создание пары
inline obj_ptr makePair (obj_ptr car, obj_ptr cdr) {
    return makeObject(T_PAIR, Pair(car, cdr));
}

//! Создание числа
inline obj_ptr makeNumber (long double n) {
    return makeObject(T_NUMBER, Number(n));
}

//! Создание булевого значения
inline obj_ptr makeBool (bool p) {
    return makeObject(T_BOOL, Bool(p));
}

#endif // TOOLS_HPP
