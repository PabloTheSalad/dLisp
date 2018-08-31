#include "predicate.hpp"
#include "../../lispTypes.hpp"
#include "tools.hpp"

obj_ptr booleanPred(obj_ptr pair, env_ptr env) {
    auto obj = pair->pair->car;
    return makeBool(obj->type == T_BOOL);
}

obj_ptr symbolPred(obj_ptr pair, env_ptr env) {
    auto obj = pair->pair->car;
    return makeBool(obj->type == T_SYMBOL);
}

obj_ptr nullPred(obj_ptr pair, env_ptr env) {
    auto obj = pair->pair->car;
    return makeBool(obj->type == T_EMPTY);
}

obj_ptr pairPred(obj_ptr pair, env_ptr env) {
    auto obj = pair->pair->car;
    return makeBool(obj->type == T_PAIR);
}

obj_ptr numberPred(obj_ptr pair, env_ptr env) {
    auto obj = pair->pair->car;
    return makeBool(obj->type == T_NUMBER);
}

obj_ptr stringPred(obj_ptr pair, env_ptr env) {
    auto obj = pair->pair->car;
    return makeBool(obj->type == T_STRING);
}

obj_ptr procedurePred(obj_ptr pair, env_ptr env) {
    auto obj = pair->pair->car;
    return makeBool(obj->type == T_PROC);
}

obj_ptr eqPred(obj_ptr obj, env_ptr env) {
    return makeBool(obj->at(0) == obj->at(1));
}

obj_ptr eqvPred(obj_ptr obj, env_ptr env) {
    auto p = equalPred(obj, env);
    if (p->boolean) return p;
    else return makeBool(*obj->at(0) == *obj->at(1));
}

//непроверено
obj_ptr equalPred(obj_ptr obj, env_ptr env) {
    auto first = obj->at(0);
    auto second = obj->at(1);
    if (first->type == T_PAIR and second->type == T_PAIR) {
        auto new_list1 = singletonList(first->pair->cdr);
        new_list1->append(second->pair->cdr);
        auto p1 = equalPred(new_list1, env);
        auto new_list2 = singletonList(first->pair->car);
        new_list2->append(second->pair->car);
        auto p2 = equalPred(new_list2, env);
        if (p1->boolean and p2->boolean) return p1;
        else return makeBool(false);
    } else return eqvPred(obj, env);
}

obj_ptr notPred(obj_ptr obj, env_ptr env) {
    auto p = obj->pair->car->isTrue();
    return makeBool(!p);
}
