#include "tools.hpp"
#include "lispTypes.hpp"

/*!
 * \brief Создает копию объекта, если возможно
 * \param obj Объект который необходимо скопировать
 * \return Возвращает копию объекта
 *
 * Не копирует неизменяемые объекты
 */
obj_ptr copyObject(obj_ptr obj) {
    if (!obj->isMutable) return obj;
    else if (obj->type == T_PAIR) {
        obj_ptr new_list = singletonList(copyObject(obj->pair().car));
        obj = obj->pair().cdr;
        for (; obj->type == T_PAIR; obj = obj->pair().cdr) {
            new_list->append(copyObject(obj->pair().car));
        }
        if (obj->type != T_EMPTY) {
            auto nl_ptr = new_list;
            for (;nl_ptr->pair().cdr->type != T_EMPTY; nl_ptr = nl_ptr->pair().cdr);
            nl_ptr->pair().cdr = obj;
        }
        return new_list;
    } else return makeObject(obj->type, obj_ptr(obj));
}
