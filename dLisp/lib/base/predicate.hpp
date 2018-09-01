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

    const FuncTable predicateFuncTable = {
        {"boolean?", c(booleanPred, 1)},
        {"symbol?", c(symbolPred, 1)},
        {"null?", c(emptyListPred, 1)},
        {"pair?", c(pairPred, 1)},
        {"number?", c(numberPred, 1)},
        {"string?", c(stringPred, 1)},
        {"procedure?", c(procedurePred, 1)},
        {"eq?", c(eqPred, 2)},
        {"eqv?", c(eqvPred, 2)},
        {"equal?", c(equalPred, 2)},
        {"not", c(logicNot, 1)},
        {"or", c(logicOr, 2)},
        {"and", c(logicAnd, 2)},
        {"xor", c(logicXor, 2)}
    };

}

#endif /* PREDICATE_HPP */

