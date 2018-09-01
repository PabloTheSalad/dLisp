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

#include "../FuncTable.hpp"

obj_ptr add(obj_ptr obj);
obj_ptr sub(obj_ptr obj);
obj_ptr multiply(obj_ptr obj);
obj_ptr division(obj_ptr obj);
obj_ptr div(obj_ptr obj);
obj_ptr mod(obj_ptr obj);
obj_ptr realPred(obj_ptr obj);
obj_ptr integerPred(obj_ptr obj);
obj_ptr equalNumPred(obj_ptr obj);
obj_ptr lessNumPred(obj_ptr obj);
obj_ptr greaterNumPred(obj_ptr obj);
obj_ptr lteNumPred(obj_ptr obj);
obj_ptr gteNumPred(obj_ptr obj);

namespace Base {

    const FuncTable arithmeticFuncTable = {
        {"+", v(add, 1, 0)},
        {"-", v(sub, 1, 0)},
        {"*", v(multiply, 1, 0)},
        {"/", v(division, 1, 0)},
        {"div", v(div, 1, 0)},
        {"mod", v(mod, 1, 0)},
        {"real?", c(realPred, 1)},
        {"integer?", c(integerPred, 1)},
        {"=", c(equalNumPred, 2)},
        {"<", c(lessNumPred, 2)},
        {">", c(greaterNumPred, 2)},
        {"<=", c(lteNumPred, 2)},
        {">=", c(gteNumPred, 2)}
    };

}

#endif /* ARITHMETIC_HPP */

