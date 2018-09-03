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

    FuncTable arithmeticFuncTable();

}

#endif /* ARITHMETIC_HPP */

