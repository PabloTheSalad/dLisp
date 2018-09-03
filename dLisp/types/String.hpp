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
struct String : public std::string {
    String () : std::string() {}
    String (std::string& str) : std::string(str) {}
    String (const char* str) : std::string(str) {}
};

using Symbol = String;

#endif /* STRING_T_HPP */

