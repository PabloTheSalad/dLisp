#include <cstddef>

#include "MemoryManager.hpp"
#include "types/LispCell.hpp"
#include "repl.hpp"
#include "exceptions.hpp"
#include <cstdlib>
#include <iostream>
#include <functional>
#include <algorithm>

//! Cсылка на глобальный менеджер памяти
static MemoryManager* memoryManager;

//! Возвращает ссылку на глобальный менеджер памяти
MemoryManager* getMemoryManager() {
    return memoryManager;
}

/*!
 * \brief Конструктор менеджера памяти
 * 
 * В данном конструкторе выделяется память для блока памяти 
 */
MemoryManager::MemoryManager()
    :  memBlocks(0), objectIndex(new ObjectIndex(this)), freeCells() {
    nextIndex_ = RESERVED_IDX + 1;
    auto firstBlock = allocateNextMemoryBlock();
    firstBlock->at(0) = LispCell(T_EMPTY, 0);
    firstBlock->at(1) = LispCell(T_BOOL, Bool(0));
    firstBlock->at(2) = LispCell(T_BOOL, Bool(1));
    firstBlock->at(3) = LispCell(T_SPECIAL, Special(UNSPEC));
    firstBlock->at(4) = LispCell(T_SPECIAL, Special(INF));
    firstBlock->at(5) = LispCell(T_SPECIAL, Special(NAN));
}

MemoryManager::~MemoryManager() {
    mEnableCounter = false;
    delete objectIndex;
    for (auto block : memBlocks) {
        for (LispCell& cell : *block) {
            cell.clear();
        }
        delete block;
    }
}

MemoryBlock* MemoryManager::allocateNextMemoryBlock() {
    nAllocatedBlocks++;
    auto newBlock = new MemoryBlock;
    memBlocks.push_back(newBlock);
    return newBlock;
}

void MemoryManager::expandMemory() {
    allocateNextMemoryBlock();
}

size_t MemoryManager::getAllocatedBlocksCount() const noexcept {
    return nAllocatedBlocks;
}

size_t MemoryManager::getFreeCellsCount() const noexcept {
    size_t count = 0;
    for (size_t i = 0; i < nAllocatedBlocks * BLOCK_SIZE; i++) {
        LispCell& obj = getObject(i);
        if (obj.type == T_EMPTY) count++;
    }
    return count;
}

MemoryManager* initializeMemoryManager() {
    if (memoryManager == nullptr)
        memoryManager = new MemoryManager();
    return memoryManager;
}

LispCell& MemoryManager::nextCell() {
    if (freeCells.empty()) {
        index_t buffer = nextIndex_;
        nextIndex_++;
        if (buffer >= nAllocatedBlocks * BLOCK_SIZE) expandMemory();
        return getObject(buffer);
    } else {
        auto cellBuffer = freeCells.top();
        freeCells.pop();
        return cellBuffer;
    }
}

LispCell& MemoryManager::allocateCell(LispCell&& obj) {
    if (obj.type == T_EMPTY or obj.type == T_BOOL
            or obj.type == T_SPECIAL) {
        LispCell* result = nullptr;
        if (obj.type == T_EMPTY)
            result = &(*memBlocks[0])[0];
        else if (obj.type == T_BOOL)
            result = &(*memBlocks[0])[(obj.boolean()? 1 : 0) + 1];
        else if (obj.type == T_SPECIAL)
            result = &(*memBlocks[0])[obj.special().type + 3];
        obj.clear();
        return *result;
    }

    bool objIsMutable = obj.isMutable;
    if (!objIsMutable and obj.type != T_PROC) {
        auto [isExist, findedObj] = objectIndex->findObject(obj);
        if (isExist) {
            obj.clear();
            return findedObj;
        }
    }

    LispCell& newObject = nextCell();
    newObject = std::move(obj);
    newObject.refCounter = 0;
    if (!objIsMutable and obj.type != T_PROC) objectIndex->addObject(newObject);
    return newObject;
}



void MemoryManager::signalDeleteObject(LispCell* cell) {
    if (cell->type != T_EMPTY and cell->type != T_BOOL
        and cell->type != T_SPECIAL and mEnableCounter) {
        cell->refCounter--;
        if (cell->refCounter == 0) {
            freeCells.push(*cell);
            if (!cell->isMutable) objectIndex->deleteObject(*cell);
            cell->clear();
        }
    }
}

void MemoryManager::signalCreateObject(LispCell* cell) {
    cell->refCounter++;
}

LispCell& MemoryManager::getObject(index_t idx) const {
    return (*memBlocks[idx/BLOCK_SIZE])[idx % BLOCK_SIZE];
}
