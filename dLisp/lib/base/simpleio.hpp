/*!
 * \file
 * \author Павел Коваленко
 * \date 13 августа 2018 г., 11:44
 *
 * Данный файл содержит описание библиотечных процедур ввода-вывода
 */

#ifndef SIMPLEIO_HPP
#define SIMPLEIO_HPP

#include "../FuncTable.hpp"

obj_ptr display(obj_ptr);
obj_ptr write(obj_ptr);
obj_ptr newLine(obj_ptr);
obj_ptr print(obj_ptr);
obj_ptr read(obj_ptr);

namespace Base {

    FuncTable simpleIoFuncTable();

}

#endif // SIMPLEIO_HPP
