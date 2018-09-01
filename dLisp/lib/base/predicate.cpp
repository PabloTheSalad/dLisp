#include "predicate.hpp"
#include "../../lispTypes.hpp"
#include "tools.hpp"

//! Проверка является ли объект будевым значением
obj_ptr booleanPred(obj_ptr pair) {
    auto obj = pair->pair->car;
    return makeBool(obj->type == T_BOOL);
}

//! Проверка является ли объект символом
obj_ptr symbolPred(obj_ptr pair) {
    auto obj = pair->pair->car;
    return makeBool(obj->type == T_SYMBOL);
}

//! Проверка является ли объект пустым списком
obj_ptr emptyListPred(obj_ptr pair) {
    auto obj = pair->pair->car;
    return makeBool(obj->type == T_EMPTY);
}

//! Проверка является ли объект парой
obj_ptr pairPred(obj_ptr pair) {
    auto obj = pair->pair->car;
    return makeBool(obj->type == T_PAIR);
}

//! Проверка является ли объект числом
obj_ptr numberPred(obj_ptr pair) {
    auto obj = pair->pair->car;
    return makeBool(obj->type == T_NUMBER);
}

//! Проверка является ли объект строкой
obj_ptr stringPred(obj_ptr pair) {
    auto obj = pair->pair->car;
    return makeBool(obj->type == T_STRING);
}

//! Проверка является ли объект процедурой
obj_ptr procedurePred(obj_ptr pair) {
    auto obj = pair->pair->car;
    return makeBool(obj->type == T_PROC);
}

//! Проверка равенства объектов
obj_ptr eqPred(obj_ptr obj) {
    return makeBool(obj->at(0) == obj->at(1));
}

//! Проверка равенства значений объектов
obj_ptr eqvPred(obj_ptr obj) {
    auto p = equalPred(obj);
    if (p->boolean) return p;
    else return makeBool(*obj->at(0) == *obj->at(1));
}

//! Проверка равенства значений обхектов в глубину (?)
obj_ptr equalPred(obj_ptr obj) {
    auto first = obj->at(0);
    auto second = obj->at(1);
    if (first->type == T_PAIR and second->type == T_PAIR) {
        auto new_list1 = singletonList(first->pair->cdr);
        new_list1->append(second->pair->cdr);
        auto p1 = equalPred(new_list1);
        auto new_list2 = singletonList(first->pair->car);
        new_list2->append(second->pair->car);
        auto p2 = equalPred(new_list2);
        if (p1->boolean and p2->boolean) return p1;
        else return makeBool(false);
    } else return eqvPred(obj);
}

//! Логическое НЕ
obj_ptr logicNot(obj_ptr obj) {
    return makeBool(!obj->at(0)->isTrue());
}

//! Логическое ИЛИ
obj_ptr logicOr(obj_ptr args) {
    return makeBool(args->at(0)->isTrue() or args->at(1)->isTrue());
}

//! Логическое И
obj_ptr logicAnd(obj_ptr args) {
    return makeBool(args->at(0)->isTrue() and args->at(1)->isTrue());
}

//! Логическое исключающее ИЛИ
obj_ptr logicXor(obj_ptr args) {
    return makeBool(args->at(0)->isTrue() xor args->at(1)->isTrue());
}
