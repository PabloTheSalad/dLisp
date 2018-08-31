/*! 
 * \file
 * \author Павел Коваленко
 * \date 2 августа 2018 г., 2:32
 */

#ifndef LISP_TYPE_FLAG_HPP
#define LISP_TYPE_FLAG_HPP

//! Флаги типов lisp-объектов
enum LispTypeFlag : char {
    T_EMPTY, ///< Флаг типа пустого списка ()
    T_SPECIAL, ///< Флаг специального типа
    T_BOOL, ///< Флаг логического (булевого) типа
    T_NUMBER, ///< Флаг числового типа
    T_STRING, ///< Флаг типа строки
    T_SYMBOL, ///< Флаг типа символа
    T_PAIR, ///< Флаг типа пары
    T_PROC, ///< Флаг типа процедуры
    T_ENV ///< Флаг типа окружения
};    

#endif /* LISP_TYPE_FLAG_HPP */

