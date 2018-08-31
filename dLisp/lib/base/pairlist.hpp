#ifndef PAIRLIST_HPP
#define PAIRLIST_HPP

#include "../FuncTable.hpp"

obj_ptr cons(obj_ptr obj, env_ptr env);
obj_ptr car(obj_ptr obj, env_ptr env);
obj_ptr cdr(obj_ptr obj, env_ptr env);
obj_ptr listPred(obj_ptr obj, env_ptr env);
obj_ptr list(obj_ptr obj, env_ptr env);
obj_ptr length(obj_ptr obj, env_ptr env);
obj_ptr append(obj_ptr obj, env_ptr env);

namespace BasePairlist {

    const FuncTable funcTable = {
        {"cons", c(cons, 2)},
        {"car", c(car, 1)},
        {"cdr", c(cdr, 1)},
        {"list?", c(listPred, 1)},
        {"list", v(list, 0, SIZE_MAX)},
        {"length", c(length, 1)},
        {"append", v(append, 0, SIZE_MAX)}
    };

}

#endif // PAIRLIST_HPP
