#include "pairlist.hpp"
#include "types/Pair.hpp"
#include "exceptions.hpp"
#include "repl.hpp"
#include "tools.hpp"

obj_ptr cons(obj_ptr obj, env_ptr env) {
    return makePair(obj->at(0), obj->at(1));
}

obj_ptr car(obj_ptr obj, env_ptr env) {
    assertArgsType(T_PAIR, obj);
    return obj->pair->car->pair->car;
}

obj_ptr cdr(obj_ptr obj, env_ptr env) {
    assertArgsType(T_PAIR, obj);
    return obj->pair->car->pair->cdr;
}

obj_ptr listPred(obj_ptr obj, env_ptr env) {
    return makeBool(obj->pair->car->isList());
}

obj_ptr list(obj_ptr args, env_ptr env) {
    size_t len = args->len() - 1;
    if (len == 0) return emptyList();
    else {
        obj_ptr list = singletonList(args->pair->car);
        forAllInList(args->pair->cdr, [&list](auto obj){
            list->append(obj);
        });
        return list;
    }
}

obj_ptr length(obj_ptr obj, env_ptr env) {
    if (obj->pair->car->type != T_EMPTY and !obj->pair->car->isList()) {
        LispException err("Wrong type argument in position ", obj->pair->car);
        err.errorString += "1: " + objectAsString(obj->pair->car);
        err.addProc = true;
        throw err;
    }
    return makeNumber(obj->pair->car->len() - 1);
}

//TODO: Не работает более чем с двумя списками
obj_ptr append(obj_ptr args, env_ptr env) {
    size_t args_len = args->len() - 1;
    if (args_len == 0) return emptyList();
    else if (args_len == 1) return copyObject(args->pair->car);

    bool p = true;
    int i = 0;
    obj_ptr arg_ptr = args;
    for (; arg_ptr->pair->cdr->type != T_EMPTY; arg_ptr = arg_ptr->pair->cdr) {
        p = p and arg_ptr->pair->car->isList();
        i++;
        if (!p) break;
    }

    if (!p) {
        LispException err("Wrong type argument in position ", args->pair->car);
        err.errorString += std::to_string(i) + ": " + objectAsString(args->pair->car);
        err.addProc = true;
        throw err;
    }

    obj_ptr new_list = copyObject(args->pair->car);
    obj_ptr nl_ptr = new_list;
    for (; nl_ptr->pair->cdr->type != T_EMPTY; nl_ptr = nl_ptr->pair->cdr);
    args = args->pair->cdr;
    for (; args->type == T_PAIR; args = args->pair->cdr, nl_ptr = nl_ptr->pair->cdr) {
        nl_ptr->pair->cdr = args->pair->car;
    }
    //nl_ptr->pair->cdr = args;

    return new_list;
}
