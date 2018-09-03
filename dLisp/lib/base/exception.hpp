#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include "mm_ptr.hpp"
#include "../FuncTable.hpp"

obj_ptr error(obj_ptr);

namespace Base {

    FuncTable exceptionFuncTable();

}

#endif // EXCEPTION_HPP
