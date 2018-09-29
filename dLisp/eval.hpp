/*! 
 * \file
 * \author Павел Коваленко
 * \date 28 июля 2018 г., 16:05
 */

#ifndef EVAL_HPP
#define EVAL_HPP

#include "mm_ptr.hpp"

obj_ptr evalList(obj_ptr, env_ptr&);
obj_ptr evalExpression(obj_ptr, env_ptr&);

#endif /* EVAL_HPP */

