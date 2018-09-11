#include "ObjectIndex.hpp"
#include "LispTypeFlag.hpp"
#include "types/LispCell.hpp"
#include "MemoryManager.hpp"
#include <algorithm>


/*!
 * \brief Конструктор индекса объектов
 * \param m Ссылка на блок памяти менеджера памяти
 * \param arr Массив индексов объектов память для которых должна быть выделена только один раз,
 * массив имеет длинну 5 и индексы объектов в данном порядке:
 * - индекс нулевого объекта: ()
 * - индекс false-значения: #f
 * - индекс true-значения: #t
 * - индекс неопределенного значения: #<undefined>
 * - индекс значения бесконечности: Inf
 * - индекс нечислового значения: Nan
 */
ObjectIndex::ObjectIndex(MemoryManager* m, index_t arr[6]) : objects(), memoryManager(m) {
    nullIndex = arr[0];
    boolIndex[0] = arr[1];
    boolIndex[1] = arr[2];
    specialIndex[0] = arr[3];
    specialIndex[1] = arr[4];
    specialIndex[2] = arr[5];
}

/*!
 * \brief Ищет объект в индексе
 * \param p Ссылка на пременную булевого типа в которую записывается результат 
 * поиска true - поск удачен, false - поиск неудачен
 * \param obj Ссылка на объект который требуется найти
 * 
 * Находит объект \a obj в индексе и возвращает его номер иначе записывает в 
 * p - false.
 */
index_t ObjectIndex::findObject(bool& p, LispCell& obj) {
    p = true;
    if (obj.type == T_EMPTY) return nullIndex;
    else if (obj.type == T_BOOL) return boolIndex[obj.boolean()? 1 : 0];
    else if (obj.type == T_SPECIAL) return specialIndex[obj.special().type];
    else {
        auto result = objects.find(obj);
        if (result != objects.cend()) return result->second;
        else {
            p = false;
            return 0;
        }
    }
}

//! Добавляет новый индекс объекта в объектный индекс
void ObjectIndex::addObject(LispCell& obj, index_t idx) {
    objects.emplace(obj, idx);
}

//! Удаляет объект из индекса
void ObjectIndex::deleteObject(LispCell& obj) {
    auto result = objects.find(obj);
    if (result != objects.cend()) objects.erase(result);
}
