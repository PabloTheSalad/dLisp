#ifndef SIMPLEIO_HPP
#define SIMPLEIO_HPP

#include "../FuncTable.hpp"

obj_ptr display(obj_ptr);
obj_ptr write(obj_ptr);
obj_ptr newLine(obj_ptr);
obj_ptr print(obj_ptr args);

namespace Base {

    FuncTable simpleIoFuncTable();

}

#endif // SIMPLEIO_HPP
