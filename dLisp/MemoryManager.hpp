/*! 
 * \file
 * \author Павел Коваленко
 * \date 23 июля 2018 г., 23:35
 */

#ifndef MEMORY_MANAGER_HPP
#define MEMORY_MANAGER_HPP

#include <cstring>
#include <stack>
#include <vector>
#include "ObjectIndex.hpp"

//! Размер блока памяти в байтах
const unsigned int BLOCK_SIZE = 1024;

//! Кол-во зарезервированных ячеек
const unsigned int RESERVED_IDX = 5;

//! Тип предназначеный для индексации памяти
using index_t = size_t;

using MemoryBlock = std::array<LispCell, BLOCK_SIZE>;

struct LispCell;
class Environment;

template<class T>
class mm_ptr;

using env_ptr = mm_ptr<Environment>;

/*!
 * \brief Класс предназначеный для управления памятью
 * 
 * Данный класс предназнначен для управления памятью внутри интерпретатора
 */
class MemoryManager {
    bool mEnableCounter = true;
    size_t nAllocatedBlocks = 0;
    std::vector<MemoryBlock*> memBlocks;
    index_t nextIndex_; ///< Следующий доступный индекс памяти
    ObjectIndex* objectIndex; ///< Объектный индекс
    std::stack<index_t> freeCells; ///< Очередь содержащая индексы свободныых ячеек памяти

    MemoryBlock* allocateNextMemoryBlock();
    index_t nextIndex();
    index_t collectGarbage();
    void collectGarbageDeep();
    void expandMemory();

    MemoryManager ();
    friend MemoryManager* initializeMemoryManager();

    friend void repl(MemoryManager*, env_ptr);
public:
    ~MemoryManager();
    std::vector<MemoryBlock*>& getMemBlocks() { return memBlocks; }
    index_t allocateCell(LispCell&&);
    void signalDeleteObject(index_t idx);
    void signalCreateObject(index_t idx);
    LispCell& getObject(index_t) const;
    index_t getIndex(const LispCell* obj) const;
    size_t getAllocatedBlocksCount() const noexcept;
    size_t getFreeCellsCount() const noexcept;
    void enableCounter(bool p) { mEnableCounter = p; }
};

MemoryManager* getMemoryManager();
MemoryManager* initializeMemoryManager();
void incObjectCounter(index_t obj);
void decObjectCounter(index_t obj);


#endif /* MEMORY_MANAGER_HPP */

