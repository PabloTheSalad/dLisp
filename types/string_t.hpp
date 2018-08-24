/*! 
 * \file
 * \author Павел Коваленко
 * \date 23 июля 2018 г., 22:25
 */

#ifndef STRING_T_HPP
#define STRING_T_HPP

#include <string>

/*!
 * \brief Класс lisp-типа строк
 */
struct string_t : public std::string {
    string_t () : std::string() {}
    string_t (std::string& str) : std::string(str) {}
    string_t (const char* str) : std::string(str) {}
};

typedef string_t symbol_t;

#endif /* STRING_T_HPP */

