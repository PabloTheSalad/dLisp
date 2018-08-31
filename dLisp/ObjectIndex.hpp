/*! 
 * \file
 * \author Павел Коваленко
 * \date 20 августа 2018 г., 10:47
 */

#ifndef OBJECT_INDEX_HPP
#define OBJECT_INDEX_HPP

#include <vector>
#include <array>

struct LispCell;
using index_t = size_t;

class MemoryManager;

/*!
 * \brief Класс отвечающий за опрделение существования объекта в менеджере памяти
 * 
 * Данный класс предназначен для определения того хранится ли такой же объект в менеджере памяти
 * или нет, если данный объект уже существует то вместо создания его копии используется
 * ссылка на него
 */
class ObjectIndex {
    std::vector<index_t> objects;
    MemoryManager* memoryManager;
    index_t nullIndex;
    index_t boolIndex[2];
    index_t specialIndex[3];
public:
    ObjectIndex();
    ObjectIndex(MemoryManager* m, index_t*);
    index_t findObject(bool&, const LispCell&);
    void addIndex(index_t);
    void deleteIndex(index_t);
};

#endif /* OBJECT_INDEX_HPP */

