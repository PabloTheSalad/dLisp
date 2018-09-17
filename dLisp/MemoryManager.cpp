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
MemoryManager::MemoryManager() :  memBlocks(0), freeCells() {
    nextIndex_ = RESERVED_IDX + 1;
    auto firstBlock = allocateNextMemoryBlock();
    firstBlock->at(0) = LispCell(T_EMPTY, 0);
    firstBlock->at(1) = LispCell(T_BOOL, Bool(0));
    firstBlock->at(2) = LispCell(T_BOOL, Bool(1));
    firstBlock->at(3) = LispCell(T_SPECIAL, Special(UNSPEC));
    firstBlock->at(4) = LispCell(T_SPECIAL, Special(INF));
    firstBlock->at(5) = LispCell(T_SPECIAL, Special(NAN));
    index_t obj_arr[6] = {0,1,2,3,4,5};
    objectIndex = new ObjectIndex(this, obj_arr);
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
//    collectGarbageDeep();
    allocateNextMemoryBlock();
}

index_t MemoryManager::collectGarbage() {
    index_t clearedCells = 0;
    for (index_t idx = RESERVED_IDX + 1; idx < nextIndex_ - 1; idx++) {
        LispCell& cell = getObject(idx);
        if (cell.refCounter == 0 and cell.type != T_EMPTY) {
            freeCells.push(idx);
            if (!cell.isMutable) objectIndex->deleteObject(cell);
            cell.clear();
            clearedCells++;
        }
    }
    return clearedCells;
}

void MemoryManager::collectGarbageDeep() {
    while (true) {
        if (collectGarbage() == 0) break;
    }
}

size_t MemoryManager::getAllocatedBlocksCount() const noexcept {
    return nAllocatedBlocks;
}

size_t MemoryManager::getFreeCellsCount() const noexcept {
    size_t count = 0;
    for (size_t i = 0; i < nAllocatedBlocks * BLOCK_SIZE; i++) {
        LispCell& obj = getObject(i);
        if (obj.refCounter == 0 and obj.type == T_EMPTY) count++;
    }
    return count;
}

MemoryManager* initializeMemoryManager() {
    if (memoryManager == nullptr)
        memoryManager = new MemoryManager();
    return memoryManager;
}

index_t MemoryManager::nextIndex() {
    index_t buffer = 0;
    if (freeCells.empty()) {
        buffer = nextIndex_;
        nextIndex_++;
        if (buffer >= nAllocatedBlocks * BLOCK_SIZE) expandMemory();
        return buffer;
    } else {
        buffer = freeCells.top();
        freeCells.pop();
        return buffer;
    }
}

index_t MemoryManager::allocateCell(LispCell&& obj) {
    index_t idx = 0;
    bool isExist;
    bool objIsMutable = obj.isMutable;
    if (!objIsMutable and obj.type != T_PROC) {
        std::tie(isExist, idx) = objectIndex->findObject(obj);
        if (isExist) {
            obj.clear();
            return idx;
        }
    }
    idx = nextIndex();
    LispCell& newObject = getObject(idx);
    newObject = std::move(obj);
    newObject.refCounter = 0;
    if (!objIsMutable and obj.type != T_PROC) objectIndex->addObject(newObject, idx);
    return idx;
}



void MemoryManager::signalDeleteObject(index_t idx) {
    if (idx > RESERVED_IDX and mEnableCounter) {
        LispCell& cell = getObject(idx);
        cell.refCounter--;
        if (cell.refCounter == 0) {
            freeCells.push(idx);
            if (!cell.isMutable) objectIndex->deleteObject(cell);
            cell.clear();
        }
    }
}

void MemoryManager::signalCreateObject(index_t idx) {
    if (idx > RESERVED_IDX and mEnableCounter) {
        getObject(idx).refCounter++;
    }
}

LispCell& MemoryManager::getObject(index_t idx) const {
//    if (idx > BLOCK_SIZE * nAllocatedBlocks) {
//        throw memoryError("Bad memory index");
//    }
    return (*memBlocks[idx/BLOCK_SIZE])[idx % BLOCK_SIZE];
}

index_t MemoryManager::getIndex(const LispCell* obj) const {
    for (size_t i = 0; i < nAllocatedBlocks; i++) {
        if (obj >= memBlocks[i]->data() and obj < memBlocks[i]->data() + BLOCK_SIZE) {
            return (BLOCK_SIZE * i) + static_cast<index_t>(obj - memBlocks[i]->data());
        }
    }
    throw memoryError("Bad memory pointer");
}
