/*! 
 * \file
 * \author Павел Коваленко
 * \date 13 августа 2018 г., 12:49
 */

#ifndef FUNC_TABLE_T_HPP
#define FUNC_TABLE_T_HPP

#include "types/Procedure.hpp"
#include <map>

//! Определяет таблицу функций используемую как интерфейс для библиотек
using FuncTable = std::map<const char*, std::tuple<Function, size_t, size_t>>;

//! Создание пары (функция, кол-во параметров), постоянное кол-во параметров
#define c(f, n) std::make_tuple<Function, size_t, size_t>((f), (n), (n))

//! Создание пары (функция, мин. кол-во параметров, макс. кол-во параметров), переменное кол-во параметров
#define v(f, min, max) std::make_tuple<Function, size_t, size_t>((f), (min), (max))

#endif /* FUNC_TABLE_T_HPP */

