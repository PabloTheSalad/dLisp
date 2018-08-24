/*! 
 * \file
 * \author Павел Коваленко
 * \date 14 августа 2018 г., 12:44
 * 
 * Данный файл содержит определения арифметических процедур предоставляемых
 * библиотекой base
 */

#ifndef ARITHMETIC_HPP
#define ARITHMETIC_HPP

#include "../func_table_t.hpp"

obj_ptr add(obj_ptr obj, env_ptr env);
obj_ptr sub(obj_ptr obj, env_ptr env);
obj_ptr multiply(obj_ptr obj, env_ptr env);
obj_ptr division(obj_ptr obj, env_ptr env);
obj_ptr div(obj_ptr obj, env_ptr env);
obj_ptr mod(obj_ptr obj, env_ptr env);
obj_ptr real_p(obj_ptr obj, env_ptr env);
obj_ptr int_p(obj_ptr obj, env_ptr env);
obj_ptr eq_np(obj_ptr obj, env_ptr env);
obj_ptr less_np(obj_ptr obj, env_ptr env);
obj_ptr greater_np(obj_ptr obj, env_ptr env);
obj_ptr lte_np(obj_ptr obj, env_ptr env);
obj_ptr gte_np(obj_ptr obj, env_ptr env);


namespace base_arithmetic {
    const func_table_t func_table = {
        {"+", p(add, 2)},
        {"-", p(sub, 2)},
        {"*", p(multiply, 2)},
        {"/", p(division, 2)},
        {"div", p(div, 2)},
        {"mod", p(mod, 2)},
        {"real?", p(real_p, 1)},
        {"integer?", p(int_p, 1)},
        {"=", p(eq_np, 2)},
        {"<", p(less_np, 2)},
        {">", p(greater_np, 2)},
        {"<=", p(lte_np, 2)},
        {">=", p(gte_np, 2)}
    };
}

#endif /* ARITHMETIC_HPP */

