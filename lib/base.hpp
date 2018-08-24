/*! 
 * \file
 * \author Павел Коваленко
 * \date 13 августа 2018 г., 12:07
 * 
 * Данный файл является основным для библиотеки base - базовой библиотеки
 * встроенной в интерпретатор. Base включает в себя
 * - predicate - основные предикаты типов
 * - arithmetic - основные арифметические выражения
 */

#ifndef BASE_HPP
#define BASE_HPP

#include "base/predicate.hpp"
#include "base/arithmetic.hpp"

namespace base {
    namespace predicate = base_predicate;
    namespace arithmetic = base_arithmetic;
}

#endif /* BASE_HPP */

