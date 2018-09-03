/*! 
 * \file
 * \author Павел Коваленко
 * \date 23 июля 2018 г., 22:15
 */

#ifndef NUMBER_T_HPP
#define NUMBER_T_HPP

//! Флаги типов чисел
enum NumberTypeFlag : bool {
    T_REAL, ///< Вещественное число
    T_INT ///< Целое число
};

const long double epsilon = 1e-20l;

/*!
 * \brief Класс lisp-типа чисел
 * 
 * Данный класс определяет представление чисел внутри интерпретатора 
 * и определяет основные операции для них.
 */
struct Number {
    NumberTypeFlag type;
    long double value;
    Number() : value(0) {}
    Number(NumberTypeFlag type, long double num) : type(type), value(num) {}
    Number(long double num) : type(), value(num) {}
    bool operator == (const Number& other) const;
    inline bool operator < (const Number& other) const { return value < other.value; }
    inline bool operator > (const Number& other) const { return value > other.value; }
    inline bool operator <= (const Number& other) const { return value < other.value or *this == other; }
    inline bool operator >= (const Number& other) const { return value > other.value or *this == other; }
    Number operator + (const Number& other) const;
    Number& operator += (const Number& other);
    Number operator - (const Number& other) const;
    Number& operator -= (const Number& other);
    Number operator * (const Number& other) const;
    Number& operator *= (const Number& other);
    Number operator / (const Number& other) const;
    Number& operator /= (const Number& other);
    Number operator % (const Number& other) const;
    Number& operator %= (const Number& other);
    Number div (const Number& other) const;
    Number& divInplace (const Number& rhs);
};

#endif /* NUMBER_T_HPP */

