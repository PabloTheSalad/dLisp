/*! 
 * \file
 * \author Павел Коваленко
 * \date 20 августа 2018 г., 10:47
 */

#ifndef OBJECT_INDEX_HPP
#define OBJECT_INDEX_HPP

#include <map>
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
    std::map<std::reference_wrapper<const LispCell>, index_t, std::less<const LispCell>> objects;
    MemoryManager* memoryManager;
    index_t nullIndex;
    index_t boolIndex[2];
    index_t specialIndex[3];
public:
    ObjectIndex();
    ObjectIndex(MemoryManager* m, index_t[6]);
    index_t findObject(bool&, LispCell&);
    void addObject(LispCell&, index_t);
    void deleteObject(LispCell&);
};

#endif /* OBJECT_INDEX_HPP */

