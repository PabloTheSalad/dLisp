/*! 
 * \file
 * \author Павел Коваленко
 * \date 23 июля 2018 г., 22:15
 */

#ifndef NUMBER_T_HPP
#define NUMBER_T_HPP

//! Флаги типов чисел
enum number_type_flag : bool {
    T_REAL, ///< Вещественное число
    T_INT ///< Целое число
};

const double epsilon = 0.000000001;

/*!
 * \brief Класс lisp-типа чисел
 * 
 * Данный класс определяет представление чисел внутри интерпретатора 
 * и определяет основные операции для них.
 */
struct number_t {
    number_type_flag type;
    long double value;
    number_t(number_type_flag type, long double num) : type(type), value(num) {}
    number_t(long double num) : type(), value(num) { raise_type(); }
    void raise_type();
    bool operator == (const number_t& other);
    inline bool operator < (const number_t& other) { return value < other.value; }
    inline bool operator > (const number_t& other) { return value > other.value; }
    inline bool operator <= (const number_t& other) { return value < other.value or *this == other; }
    inline bool operator >= (const number_t& other) { return value > other.value or *this == other; }
    number_t operator + (const number_t& other);
    number_t operator - (const number_t& other);
    number_t operator * (const number_t& other);
    number_t operator / (const number_t& other);
    number_t operator % (const number_t& other);
    number_t div (const number_t& other);
};

#endif /* NUMBER_T_HPP */

