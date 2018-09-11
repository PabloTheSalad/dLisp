/*! 
 * \file
 * \author Павел Коваленко
 * \date 13 августа 2018 г., 12:49
 */

#ifndef FUNC_TABLE_T_HPP
#define FUNC_TABLE_T_HPP

#include <map>

class LispCell;

template<class T>
class mm_ptr;

using obj_ptr = mm_ptr<LispCell>;

//! Тип указателя на функцию из библиотеки
using Function = obj_ptr(*)(obj_ptr);

//! Определяет таблицу функций используемую как интерфейс для библиотек
using FuncTable = std::map<const char*, std::tuple<Function, size_t, size_t>>;

//! Создание пары (функция, кол-во параметров), постоянное кол-во параметров
inline auto c(Function f, size_t n) {
    return std::make_tuple(f, n, n);
}

//! Создание пары (функция, мин. кол-во параметров, макс. кол-во параметров), переменное кол-во параметров
inline auto v(Function f, size_t min, size_t max) {
    return std::make_tuple(f, min, max);
}

#endif /* FUNC_TABLE_T_HPP */

