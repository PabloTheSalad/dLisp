#include "arithmetic.hpp"
#include "../../mm_ptr.hpp"
#include "../../lispTypes.hpp"
#include "../../exceptions.hpp"
#include "tools.hpp"
#include <iostream>


//! Сложение
obj_ptr add(obj_ptr args, env_ptr env) {
    assertArgsType(T_NUMBER, args);

    Number buf;
    forAllInList(args, [&buf](auto obj){
        buf += obj->number;
    });

    return makeNumber(buf.value);
}

//! Вычитание
obj_ptr sub(obj_ptr args, env_ptr env) {
    assertArgsType(T_NUMBER, args);

    Number buf(args->at(0)->number);
    forAllInList(args->pair->cdr, [&buf](auto obj){
        buf -= obj->number;
    });

    return makeNumber(buf.value);
}

//! Умножение
obj_ptr multiply(obj_ptr args, env_ptr env) {
    assertArgsType(T_NUMBER, args);

    Number buf(1);
    forAllInList(args, [&buf](auto obj){
        buf *= obj->number;
    });

    return makeNumber(buf.value);
}

//! Деление
obj_ptr division(obj_ptr args, env_ptr env) {
    assertArgsType(T_NUMBER, args);

    Number buf(args->at(0)->number);
    forAllInList(args->pair->cdr, [&buf](auto obj){
        buf /= obj->number;
    });

    return makeNumber(buf.value);
}

//! Целочисленное деление
obj_ptr div(obj_ptr args, env_ptr env) {
    assertArgsType(T_NUMBER, args);

    Number buf(args->at(0)->number);
    forAllInList(args->pair->cdr, [&buf](auto obj){
        buf.divInplace(obj->number);
    });

    return makeNumber(buf.value);
}

//! Остаток от деления
obj_ptr mod(obj_ptr args, env_ptr env) {
    assertArgsType(T_NUMBER, args);

    Number buf(args->at(0)->number);
    forAllInList(args->pair->cdr, [&buf](auto obj){
        buf %= obj->number;
    });

    return makeNumber(buf.value);
}

//! Предикат проверяющий является ли число вещественным
obj_ptr realPred(obj_ptr obj, env_ptr env) {
    bool p = obj->at(0)->type == T_NUMBER;
    return makeBool(p and obj->at(0)->number.type == T_REAL);
}

//! Предикат проверяющий является ли число целым
obj_ptr integerPred(obj_ptr obj, env_ptr env) {
    bool p = obj->at(0)->type == T_NUMBER;
    return makeBool(p and obj->at(0)->number.type == T_INT);
}

//! Равенство чисел
obj_ptr equalNumPred(obj_ptr obj, env_ptr env) {
    assertArgsType(T_NUMBER, obj);
    return makeBool(obj->at(0)->number == obj->at(1)->number);
}

//! Меньше
obj_ptr lessNumPred(obj_ptr obj, env_ptr env) {
    assertArgsType(T_NUMBER, obj);
    return makeBool(obj->at(0)->number < obj->at(1)->number);
}

//! Больше
obj_ptr greaterNumPred(obj_ptr obj, env_ptr env) {
    assertArgsType(T_NUMBER, obj);
    return makeBool(obj->at(0)->number > obj->at(1)->number);
}

//! Меньше либо равно
obj_ptr lteNumPred(obj_ptr obj, env_ptr env) {
    assertArgsType(T_NUMBER, obj);
    return makeBool(obj->at(0)->number <= obj->at(1)->number);
}

//! Больше либо равно
obj_ptr gteNumPred(obj_ptr obj, env_ptr env) {
    assertArgsType(T_NUMBER, obj);
    return makeBool(obj->at(0)->number >= obj->at(1)->number);
}
