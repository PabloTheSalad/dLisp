
#include "object_index.hpp"
#include "lisp_type_flag.hpp"
#include "types/lisp_t.hpp"

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
object_index::object_index(lisp_t* m, index_t* arr) : objects(), memory(m) {
    null_idx = arr[0];
    bool_idx[0] = arr[1];
    bool_idx[1] = arr[2];
    special_idx[0] = arr[3];
    special_idx[1] = arr[4];
    special_idx[2] = arr[5];
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
index_t object_index::find_object(bool& p, const lisp_t& obj) {
    p = true;
    if (obj.type == T_NULL) return null_idx;
    else if (obj.type == T_BOOL) return bool_idx[obj.boolean? 1 : 0];
    else if (obj.type == T_SPECIAL) return special_idx[obj.spec.type];
    else {
        for (auto idx : objects) {
            if (memory[idx] == obj) return idx;
        }
        p = false;
        return 0;
    }
}

//! Добавляет новый индекс объекта в объектный индекс
void object_index::add_index(index_t idx) {
    objects.push_back(idx);
}

void object_index::delete_index(index_t idx) {
    for (auto it = objects.begin(); it != objects.end(); it++) {
        if (*it == idx) {
            objects.erase(it);
            break;
        }
    }
}