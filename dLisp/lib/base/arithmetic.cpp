#include <iostream>

#include "arithmetic.hpp"
#include "../../mm_ptr.hpp"
#include "../../lispTypes.hpp"
#include "../../exceptions.hpp"
#include "tools.hpp"

FuncTable Base::arithmeticFuncTable() {
    return {
        {"+", v(add, 1, 0)},
        {"-", v(sub, 1, 0)},
        {"*", v(multiply, 1, 0)},
        {"/", v(division, 1, 0)},
        {"div", v(div, 1, 0)},
        {"mod", v(mod, 1, 0)},
        {"real?", c(realPred, 1)},
        {"integer?", c(integerPred, 1)},
        {"=", c(equalNumPred, 2)},
        {"<", c(lessNumPred, 2)},
        {">", c(greaterNumPred, 2)},
        {"<=", c(lteNumPred, 2)},
        {">=", c(gteNumPred, 2)}
    };
}


//! Сложение
obj_ptr add(obj_ptr args) {
    assertArgsType(T_NUMBER, args);

    Number buf;
    for (auto& cell : *args) {
        buf += cell->number();
    }

    return makeNumber(buf.value);
}

//! Вычитание
obj_ptr sub(obj_ptr args) {
    assertArgsType(T_NUMBER, args);
    if (args->len() == 1) {
        return makeNumber(-args->at(0)->number().value);
    } else {
        Number buf(args->at(0)->number());
        for (auto& cell : *args->pair().cdr) {
            buf -= cell->number();
        }

        return makeNumber(buf.value);
    }
}

//! Умножение
obj_ptr multiply(obj_ptr args) {
    assertArgsType(T_NUMBER, args);

    Number buf(1);
    for (auto& cell : *args) {
        buf *= cell->number();
    }

    return makeNumber(buf.value);
}

//! Деление
obj_ptr division(obj_ptr args) {
    assertArgsType(T_NUMBER, args);

    Number buf(args->at(0)->number());
    for (auto& cell : *args->pair().cdr) {
        buf /= cell->number();
    }

    return makeNumber(buf.value);
}

//! Целочисленное деление
obj_ptr div(obj_ptr args) {
    assertArgsType(T_NUMBER, args);

    Number buf(args->at(0)->number());
    for (auto& cell : *args->pair().cdr) {
        buf.divInplace(cell->number());
    }

    return makeNumber(buf.value);
}

//! Остаток от деления
obj_ptr mod(obj_ptr args) {
    assertArgsType(T_NUMBER, args);

    Number buf(args->at(0)->number());
    for (auto& cell : *args->pair().cdr) {
        buf %= cell->number();
    }

    return makeNumber(buf.value);
}

//! Предикат проверяющий является ли число вещественным
obj_ptr realPred(obj_ptr obj) {
    bool p = obj->at(0)->type == T_NUMBER;
    return makeBool(p and obj->at(0)->number().type == T_REAL);
}

//! Предикат проверяющий является ли число целым
obj_ptr integerPred(obj_ptr obj) {
    bool p = obj->at(0)->type == T_NUMBER;
    return makeBool(p and obj->at(0)->number().type == T_INT);
}

//! Равенство чисел
obj_ptr equalNumPred(obj_ptr obj) {
    assertArgsType(T_NUMBER, obj);
    return makeBool(obj->at(0)->number() == obj->at(1)->number());
}

//! Меньше
obj_ptr lessNumPred(obj_ptr obj) {
    assertArgsType(T_NUMBER, obj);
    return makeBool(obj->at(0)->number() < obj->at(1)->number());
}

//! Больше
obj_ptr greaterNumPred(obj_ptr obj) {
    assertArgsType(T_NUMBER, obj);
    return makeBool(obj->at(0)->number() > obj->at(1)->number());
}

//! Меньше либо равно
obj_ptr lteNumPred(obj_ptr obj) {
    assertArgsType(T_NUMBER, obj);
    return makeBool(obj->at(0)->number() <= obj->at(1)->number());
}

//! Больше либо равно
obj_ptr gteNumPred(obj_ptr obj) {
    assertArgsType(T_NUMBER, obj);
    return makeBool(obj->at(0)->number() >= obj->at(1)->number());
}
