/*! 
 * \file
 * \author Павел Коваленко
 * \date 13 августа 2018 г., 12:49
 */

#ifndef FUNC_TABLE_T_HPP
#define FUNC_TABLE_T_HPP

#include "types/procedure_t.hpp"
#include <map>

//! Определяет таблицу функций используемую как интерфейс для библиотек
using func_table_t = std::map<const char*, std::pair<function_t, size_t>>;

//! Создание пары (функция, кол-во параметров)
#define p(f, n) std::make_pair<function_t, size_t>(f, n)

//! Создание числа
#define make_number(n) make_object(T_NUMBER, number_t((n)))

//! Создание булевого значения
#define make_bool(p) make_object(T_BOOL, bool_t((p)))

#endif /* FUNC_TABLE_T_HPP */

