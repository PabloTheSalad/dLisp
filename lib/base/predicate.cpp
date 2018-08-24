#include "predicate.hpp"
#include "../../lisp_types.hpp"

obj_ptr boolean_p(obj_ptr pair, env_ptr env) {
    auto obj = pair->pair->car;
    return make_bool(obj->type == T_BOOL);
}

obj_ptr symbol_p(obj_ptr pair, env_ptr env) {
    auto obj = pair->pair->car;
    return make_bool(obj->type == T_SYMBOL);
}

obj_ptr null_p(obj_ptr pair, env_ptr env) {
    auto obj = pair->pair->car;
    return make_bool(obj->type == T_NULL);
}

obj_ptr pair_p(obj_ptr pair, env_ptr env) {
    auto obj = pair->pair->car;
    return make_bool(obj->type == T_PAIR);
}

obj_ptr number_p(obj_ptr pair, env_ptr env) {
    auto obj = pair->pair->car;
    return make_bool(obj->type == T_NUMBER);
}

obj_ptr string_p(obj_ptr pair, env_ptr env) {
    auto obj = pair->pair->car;
    return make_bool(obj->type == T_STRING);
}

obj_ptr procedure_p(obj_ptr pair, env_ptr env) {
    auto obj = pair->pair->car;
    return make_bool(obj->type == T_PROC);
}

obj_ptr eq_p(obj_ptr obj, env_ptr env) {
    return make_bool(obj->at(0) == obj->at(1));
}

obj_ptr eqv_p(obj_ptr obj, env_ptr env) {
    auto p = eq_p(obj, env); 
    if (p->boolean) return p;
    else return make_bool(*obj->at(0) == *obj->at(1));
}

//непроверено
obj_ptr equal_p(obj_ptr obj, env_ptr env) {
    auto first = obj->at(0);
    auto second = obj->at(1);
    if (first->type == T_PAIR and second->type == T_PAIR) {
        auto new_list1 = singleton_list(first->pair->cdr);
        new_list1->append(second->pair->cdr);
        auto p1 = equal_p(new_list1, env);
        auto new_list2 = singleton_list(first->pair->car);
        new_list2->append(second->pair->car);
        auto p2 = equal_p(new_list2, env);
        if (p1->boolean and p2->boolean) return p1;
        else return make_bool(false);
    } else return eqv_p(obj, env);
}

obj_ptr not_p(obj_ptr obj, env_ptr env) {
    auto p = obj->pair->car->is_true();
    return make_bool(!p);
}
