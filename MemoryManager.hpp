/*! 
 * \file
 * \author Павел Коваленко
 * \date 23 июля 2018 г., 23:35
 */

#ifndef MEMORY_MANAGER_HPP
#define MEMORY_MANAGER_HPP

#include <cstring>
#include <set>
#include <map>
#include <list>
#include <queue>
#include "ObjectIndex.hpp"

//! Размер блока памяти в байтах
const unsigned int BLOCK_SIZE = 512;

//! Кол-во зарезервированных ячеек
const unsigned int RESERVED_IDX = 5;

//! Тип предназначеный для индексации памяти
using index_t = size_t;

using MemoryBlock = std::array<LispCell, BLOCK_SIZE>;

struct LispCell;
class Environment;

/*!
 * \brief Класс предназначеный для управления памятью
 * 
 * Данный класс предназнначен для управления памятью внутри интерпретатора
 */
class MemoryManager {
    size_t nAllocatedBlocks = 0;
    std::vector<MemoryBlock*> memBlocks;
    index_t nextIndex_; ///< Следующий доступный индекс памяти
    ObjectIndex* objectIndex; ///< Объектный индекс
    std::list<index_t> freeCells; ///< Очередь содержащая индексы свободныых ячеек памяти

    MemoryBlock* allocateNextMemoryBlock();
    index_t nextIndex();
    index_t collectGarbage();
    void collectGarbageDeep();
    void expandMemory();

    friend void repl();
public:
    MemoryManager ();
    std::vector<MemoryBlock*>& getMemBlocks() { return memBlocks; }
    index_t allocateObject(const LispCell&);
    void signalDeleteObject(index_t idx);
    void signalCreateObject(index_t idx);
    LispCell& getObject(index_t) const;
    index_t getIndex(LispCell* obj) const;
    size_t getAllocatedBlocksCount() const;
    size_t getFreeCellsCount() const;
};

void setMemoryManager(MemoryManager* mm);
MemoryManager* getMemoryManager();
MemoryManager* initializeMemoryManager();
void incObjectCounter(index_t obj);
void decObjectCounter(index_t obj);


#endif /* MEMORY_MANAGER_HPP */

