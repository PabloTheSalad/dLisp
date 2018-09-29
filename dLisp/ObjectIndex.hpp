/*! 
 * \file
 * \author Павел Коваленко
 * \date 20 августа 2018 г., 10:47
 */

#ifndef OBJECT_INDEX_HPP
#define OBJECT_INDEX_HPP

#include <array>
#include <functional>
#include <set>
#include "types/LispCell.hpp"

struct LispCell;
using index_t = size_t;

class MemoryManager;

class LispCellComp {
    bool operator () (LispCell* lhs, LispCell* rhs) const {
        return *lhs < *rhs;
    }
};

/*!
 * \brief Класс отвечающий за опрделение существования объекта в менеджере памяти
 * 
 * Данный класс предназначен для определения того хранится ли такой же объект в менеджере памяти
 * или нет, если данный объект уже существует то вместо создания его копии используется
 * ссылка на него
 */
class ObjectIndex {
//    std::set<index_t> objects;
    std::set<std::reference_wrapper<LispCell>, std::less<LispCell>> objects;
    MemoryManager* memoryManager;
public:
    ObjectIndex();
    ObjectIndex(MemoryManager* m) : objects(), memoryManager(m) {}
    std::pair<bool, std::reference_wrapper<LispCell>> findObject(LispCell&);
    void addObject(LispCell&);
    void deleteObject(LispCell&);
};

#endif /* OBJECT_INDEX_HPP */

