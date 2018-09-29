#include "ObjectIndex.hpp"
#include "LispTypeFlag.hpp"
#include "types/LispCell.hpp"
#include "MemoryManager.hpp"
#include <algorithm>

/*!
 * \brief Ищет объект в индексе
 * \param obj Ссылка на объект который требуется найти
 * 
 * Находит объект \a obj в индексе и возвращает его номер иначе записывает в 
 * p - false.
 */
std::pair<bool, std::reference_wrapper<LispCell>> ObjectIndex::findObject(LispCell& obj) {
    bool p = true;
    auto findResult = objects.find(obj);
    if (findResult != objects.cend()) return {p, *findResult};
    else p = false;
    return std::make_pair(p, *objects.begin());
}

//! Добавляет новый индекс объекта в объектный индекс
void ObjectIndex::addObject(LispCell& obj) {
    objects.emplace(obj);
}

//! Удаляет объект из индекса
void ObjectIndex::deleteObject(LispCell& obj) {
    auto result = objects.find(obj);
    if (result != objects.cend()) objects.erase(result);
}
