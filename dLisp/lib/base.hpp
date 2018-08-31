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
#include "base/pairlist.hpp"

namespace Base {
    namespace Predicate = BasePredicate;
    namespace Arithmetic = BaseArithmetic;
    namespace Pairlist = BasePairlist;
}

#endif /* BASE_HPP */

