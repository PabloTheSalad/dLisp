#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include "mm_ptr.hpp"
#include "../FuncTable.hpp"

obj_ptr error(obj_ptr);

namespace Base {

    const FuncTable exceptionFuncTable = {
        {"error", v(error, 1, 0)}
    };

}

#endif // EXCEPTION_HPP
