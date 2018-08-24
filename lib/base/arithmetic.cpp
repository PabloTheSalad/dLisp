#include "arithmetic.hpp"
#include "../../mm_ptr.hpp"
#include "../../lisp_types.hpp"
#include "../../exceptions.hpp"


//! Сложение
obj_ptr add(obj_ptr obj, env_ptr env) {
    assert_args_type(T_NUMBER, obj);
    return make_number(obj->at(0)->number + obj->at(1)->number);
}

//! Вычитание
obj_ptr sub(obj_ptr obj, env_ptr env) {
    assert_args_type(T_NUMBER, obj);
    return make_number(obj->at(0)->number - obj->at(1)->number);
}

//! Умножение
obj_ptr multiply(obj_ptr obj, env_ptr env) {
    assert_args_type(T_NUMBER, obj);
    return make_number(obj->at(0)->number * obj->at(1)->number);
}

//! Деление
obj_ptr division(obj_ptr obj, env_ptr env) {
    assert_args_type(T_NUMBER, obj);
    return make_number(obj->at(0)->number / obj->at(1)->number);
}

//! Целочисленное деление
obj_ptr div(obj_ptr obj, env_ptr env) {
    assert_args_type(T_NUMBER, obj);
    return make_number(obj->at(0)->number.div(obj->at(1)->number));
}

//! Остаток от деления
obj_ptr mod(obj_ptr obj, env_ptr env) {
    assert_args_type(T_NUMBER, obj);
    return make_number(obj->at(0)->number % obj->at(1)->number);
}

//! Предикат проверяющий является ли число вещественным
obj_ptr real_p(obj_ptr obj, env_ptr env) {
    bool p = obj->at(0)->type == T_NUMBER;
    return make_bool(p and obj->at(0)->number.type == T_REAL);
}

//! Предикат проверяющий является ли число целым
obj_ptr int_p(obj_ptr obj, env_ptr env) {
    bool p = obj->at(0)->type == T_NUMBER;
    return make_bool(p and obj->at(0)->number.type == T_INT);
}

//! Равенство чисел
obj_ptr eq_np(obj_ptr obj, env_ptr env) {
    assert_args_type(T_NUMBER, obj);
    return make_bool(obj->at(0)->number == obj->at(1)->number);
}

//! Меньше
obj_ptr less_np(obj_ptr obj, env_ptr env) {
    assert_args_type(T_NUMBER, obj);
    return make_bool(obj->at(0)->number < obj->at(1)->number);
}

//! Больше
obj_ptr greater_np(obj_ptr obj, env_ptr env) {
    assert_args_type(T_NUMBER, obj);
    return make_bool(obj->at(0)->number > obj->at(1)->number);
}

//! Меньше либо равно
obj_ptr lte_np(obj_ptr obj, env_ptr env) {
    assert_args_type(T_NUMBER, obj);
    return make_bool(obj->at(0)->number <= obj->at(1)->number);
}

//! Больше либо равно
obj_ptr gte_np(obj_ptr obj, env_ptr env) {
    assert_args_type(T_NUMBER, obj);
    return make_bool(obj->at(0)->number >= obj->at(1)->number);
}