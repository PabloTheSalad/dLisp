#ifndef DLISPVM_H
#define DLISPVM_H

#include <stack>
#include "lispTypes.hpp"

using byte = unsigned char;

struct StackCell {
    enum CellType : byte {
        T_EMPTY, ///< Флаг типа пустого списка ()
        T_SPECIAL, ///< Флаг специального типа
        T_BOOL, ///< Флаг логического (булевого) типа
        T_NUMBER, ///< Флаг числового типа
        T_STRING, ///< Флаг типа строки
        T_SYMBOL, ///< Флаг типа символа
        T_PAIR, ///< Флаг типа пары
        T_PROC, ///< Флаг типа процедуры
        T_FRAME
    } type;
    void* value;

    StackCell() : value(nullptr) {}
    StackCell(CellType t, void* v) : type(t), value(v) {}

    template<class T>
    T& getValue() { return static_cast<T*>(value); }
};

enum class ByteCode {
    Value,
    If,
    While,
};

class DlispVM {
     std::stack<StackCell> valueStack;
     byte* code;
     byte* currentCommand;
     size_t commandCounter;
};

#endif // DLISPVM_H
