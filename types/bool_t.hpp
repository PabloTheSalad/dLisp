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
class bool_t {
    bool value;
public: 
    bool_t () : value(0) {}
    bool_t (bool p) : value(p) {}
    operator bool() const { return value; }
    inline bool operator == (const bool_t& other) { return value == other.value; } 
    inline bool operator < (const bool_t& other) { return value < other.value; }
};

#endif /* BOOL_T_HPP */

