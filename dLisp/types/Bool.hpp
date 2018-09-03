/*! 
 * \file
 * \author Павел Коваленко
 * \date 27 июля 2018 г., 22:07
 */

#ifndef BOOL_T_HPP
#define BOOL_T_HPP

/*!
 * \brief Класс булевого lisp-типа
 */
class Bool {
    bool value;
public: 
    Bool () noexcept : value(0) {}
    Bool (bool p) noexcept : value(p) {}
    operator bool() const { return value; }
    inline bool operator == (const Bool& other) { return value == other.value; }
    inline bool operator < (const Bool& other) { return value < other.value; }
};

#endif /* BOOL_T_HPP */

