/* 
 * File:   predicate.hpp
 * Author: pablo
 *
 * Created on 13 августа 2018 г., 11:44
 */

#ifndef PREDICATE_HPP
#define PREDICATE_HPP

#include "../FuncTable.hpp"

obj_ptr booleanPred(obj_ptr, env_ptr);
obj_ptr symbolPred(obj_ptr, env_ptr);
obj_ptr nullPred(obj_ptr, env_ptr);
obj_ptr pairPred(obj_ptr, env_ptr);
obj_ptr numberPred(obj_ptr, env_ptr);
obj_ptr stringPred(obj_ptr, env_ptr);
obj_ptr procedurePred(obj_ptr, env_ptr);
obj_ptr eqvPred(obj_ptr, env_ptr);
obj_ptr eqPred(obj_ptr, env_ptr);
obj_ptr equalPred(obj_ptr, env_ptr);

obj_ptr notPred(obj_ptr, env_ptr);

namespace BasePredicate {

    const FuncTable func_table = {
        {"boolean?", c(booleanPred, 1)},
        {"symbol?", c(symbolPred, 1)},
        {"null?", c(nullPred, 1)},
        {"pair?", c(pairPred, 1)},
        {"number?", c(numberPred, 1)},
        {"string?", c(stringPred, 1)},
        {"procedure?", c(procedurePred, 1)},
        {"eq?", c(eqPred, 2)},
        {"eqv?", c(eqvPred, 2)},
        {"equal?", c(equalPred, 2)},
        {"not", c(notPred, 1)}
    };

}

#endif /* PREDICATE_HPP */

