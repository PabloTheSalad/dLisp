/* 
 * File:   predicate.hpp
 * Author: pablo
 *
 * Created on 13 августа 2018 г., 11:44
 */

#ifndef PREDICATE_HPP
#define PREDICATE_HPP

#include "../FuncTable.hpp"

obj_ptr booleanPred(obj_ptr);
obj_ptr symbolPred(obj_ptr);
obj_ptr emptyListPred(obj_ptr);
obj_ptr pairPred(obj_ptr);
obj_ptr numberPred(obj_ptr);
obj_ptr stringPred(obj_ptr);
obj_ptr procedurePred(obj_ptr);
obj_ptr eqvPred(obj_ptr);
obj_ptr eqPred(obj_ptr);
obj_ptr equalPred(obj_ptr);

obj_ptr logicNot(obj_ptr);
obj_ptr logicOr(obj_ptr args);
obj_ptr logicAnd(obj_ptr args);
obj_ptr logicXor(obj_ptr args);

namespace Base {

    FuncTable predicateFuncTable();

}

#endif /* PREDICATE_HPP */

