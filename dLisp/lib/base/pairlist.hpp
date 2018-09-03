#ifndef PAIRLIST_HPP
#define PAIRLIST_HPP

#include "../FuncTable.hpp"

obj_ptr cons(obj_ptr obj);
obj_ptr car(obj_ptr obj);
obj_ptr cdr(obj_ptr obj);
obj_ptr listPred(obj_ptr obj);
obj_ptr list(obj_ptr obj);
obj_ptr length(obj_ptr obj);
obj_ptr append(obj_ptr obj);

namespace Base {

    FuncTable pairlistFuncTable();

}

#endif // PAIRLIST_HPP
