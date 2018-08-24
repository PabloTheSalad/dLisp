/* 
 * File:   predicate.hpp
 * Author: pablo
 *
 * Created on 13 августа 2018 г., 11:44
 */

#ifndef PREDICATE_HPP
#define PREDICATE_HPP

#include "../func_table_t.hpp"

obj_ptr boolean_p(obj_ptr, env_ptr);
obj_ptr symbol_p(obj_ptr, env_ptr);
obj_ptr null_p(obj_ptr, env_ptr);
obj_ptr pair_p(obj_ptr, env_ptr);
obj_ptr number_p(obj_ptr, env_ptr);
obj_ptr string_p(obj_ptr, env_ptr);
obj_ptr procedure_p(obj_ptr, env_ptr);
obj_ptr eqv_p(obj_ptr, env_ptr);
obj_ptr eq_p(obj_ptr, env_ptr);
obj_ptr equal_p(obj_ptr, env_ptr);

obj_ptr not_p(obj_ptr, env_ptr);

namespace base_predicate {

    const func_table_t func_table = {
        {"boolean?", p(boolean_p, 1)},
        {"symbol?", p(symbol_p, 1)},
        {"null?", p(null_p, 1)},
        {"pair?", p(pair_p, 1)},
        {"number?", p(number_p, 1)},
        {"string?", p(string_p, 1)},
        {"procedure?", p(procedure_p, 1)},
        {"eq?", p(eq_p, 2)},
        {"eqv?", p(eqv_p, 2)},
        {"equal?", p(equal_p, 2)},
        {"not", p(not_p, 1)}
    };

}

#endif /* PREDICATE_HPP */

