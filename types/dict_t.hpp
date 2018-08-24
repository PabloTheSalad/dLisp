/*! 
 * \file
 * \author Павел Коваленко
 * \date 23 июля 2018 г., 22:29
 */

#ifndef DICT_T_HPP
#define DICT_T_HPP

#include <map>
#include "string_t.hpp"
#include "../mm_ptr.hpp"

//! Отвечает за хранение пар символ-значение в окружениях
using dict_t = std::map<obj_ptr, obj_ptr>;

#endif /* DICT_T_HPP */

