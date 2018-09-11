/*!
 * \file
 * \author Павел Коваленко
 * \date 5 сентября 2018 г., 00:00
 *
 * Данный файл содержит описание библиотечных процедур для обработки и вызова ошибок
 */

#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include "mm_ptr.hpp"
#include "../FuncTable.hpp"

obj_ptr error(obj_ptr);

namespace Base {

    FuncTable exceptionFuncTable();

}

#endif // EXCEPTION_HPP
