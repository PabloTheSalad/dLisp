#include "tools.hpp"
#include "lispTypes.hpp"

void forAllInList(obj_ptr obj, std::function<void(obj_ptr&)> func) {
    for (; obj->type != T_EMPTY; obj = obj->pair->cdr) func(obj->pair->car);
}

obj_ptr copyObject(obj_ptr obj) {
    if (!obj->isMutable) return obj;
    else if (obj->type == T_PAIR) {
        obj_ptr new_list = singletonList(copyObject(obj->pair->car));
        obj = obj->pair->cdr;
        for (; obj->type == T_PAIR; obj = obj->pair->cdr) {
            new_list->append(copyObject(obj->pair->car));
        }
        if (obj->type != T_EMPTY) {
            auto nl_ptr = new_list;
            for (;nl_ptr->pair->cdr->type != T_EMPTY; nl_ptr = nl_ptr->pair->cdr);
            nl_ptr->pair->cdr = obj;
        }
        return new_list;
    } else return makeObject(obj->type, obj_ptr(obj));
}
