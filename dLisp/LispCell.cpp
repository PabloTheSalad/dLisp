#include "lispTypes.hpp"
#include "exceptions.hpp"
#include "types/LispCell.hpp"
#include "mm_ptr.hpp"

template <>
LispCell::LispCell (LispTypeFlag type, Environment&& value)
    : type(type), isMutable(true),
      value(new Environment(std::move(value))) {}

template <>
LispCell::LispCell (LispTypeFlag type, String&& value)
    : type(type), isMutable(false),
      value(new String(std::move(value))) {}

template <>
LispCell::LispCell (LispTypeFlag type, Procedure&& value)
    : type(type), isMutable(false),
      value(new Procedure(std::move(value))) {}

template <>
LispCell::LispCell (LispTypeFlag type, Pair&& value)
    : type(type), isMutable(true),
      value(new Pair(std::move(value))) {}

bool LispCell::operator == (const LispCell& other) {
    if (type != other.type) return false;
    switch (type) {
        case T_EMPTY:
            return true;
        case T_BOOL:
            return boolean() == other.boolean();
        case T_NUMBER:
            return number() == other.number();
        case T_SPECIAL:
            return special() == other.special();
        case T_STRING:
        case T_SYMBOL:
            return string() == other.string();
        case T_ENV:
            return environment() == other.environment();
        case T_PROC:
            return procedure() == other.procedure();
        case T_PAIR:
            return pair() == other.pair();
    }
}

/*!
 * \brief Очистка ячейки от значений и удаление всех связанных значений
 */
void LispCell::clear() {
    switch (type) {
        case T_EMPTY:
            break;
        case T_SPECIAL:
            delete static_cast<Special*>(value);
            break;
        case T_BOOL:
            delete static_cast<Bool*>(value);
            break;
        case T_NUMBER:
            delete static_cast<Number*>(value);
            break;
        case T_STRING:
        case T_SYMBOL:
            delete static_cast<String*>(value);
            break;
        case T_PAIR:
            delete static_cast<Pair*>(value);
            break;
        case T_PROC:
            delete procedure().formalArgs;
            delete static_cast<Procedure*>(value);
            break;
        case T_ENV:
            delete environment().symbols;
            delete static_cast<Environment*>(value);
            break;
    }
    *this = LispCell();
}


//! Если ячейка представляет из себя список значений добавляет obj в конец списка
void LispCell::append(obj_ptr obj) {
    if (pair().cdr->type == T_EMPTY) {
        pair().cdr = singletonList(obj);
    } else pair().cdr->append(obj);
}

//! Создает список с одним значением
obj_ptr singletonList(obj_ptr obj) {
    return makeObject(T_PAIR, Pair(obj, emptyList()));
}

//! Создает пустой список
obj_ptr emptyList() {
    return makeObject(T_EMPTY, 0);
}

/*!
 * \brief Позволяет проверить принадлежность всех элементов списка определенному типу
 * \param type Тип которому должны соответствовать все элементы списка
 * \param list Список для проверки
 * \return Возвращает true если все элементы принадлежать типу type,
 * иначе возвращает false
 */
bool checkListType(LispTypeFlag type, obj_ptr list) {
    if(list->type != T_PAIR and list->type != T_EMPTY) return false;
    for (; list->type != T_EMPTY; list = list->pair().cdr) {
        if(list->pair().car->type != type) return false;
    }
    return true;
}

//! Возвращает последний элемент списка
obj_ptr LispCell::end() {
    if (pair().cdr->type != T_PAIR) return pair().cdr;
    else return pair().cdr->end();
}

//! Возвращает длину списка
size_t LispCell::len() {
    obj_ptr ptr = getObject(this);
    size_t i = 1;
    for (; ptr->type == T_PAIR; ptr = ptr->pair().cdr) i++;
    return i;
}

//! Возвращает элемент списка находящийся на n-ном месте (считая с нуля)
//! Если n отрицательно возвращает элементы начиная с конца -1 - предпоследний элемент
obj_ptr LispCell::at(int n) {
    if (n < 0) n = len() + n - 1;
    if (n == 0) return pair().car;
    else return pair().cdr->at(n-1);
}

//! Проверяет является ли тип хранящийся в ячейке атомарным
bool LispCell::isAtom() {
    return type != T_PAIR;
}

//! Проверяет является ли тип хранящийся в ячейке самовычислимым
bool LispCell::isSelfEvaluating() {
    return type != T_PAIR and type != T_SYMBOL;
}

//! Проверяет эквивалентен ли тип хранящийся в ячейке логической истине
bool LispCell::isTrue() {
    return !(type == T_BOOL and !boolean());
}

//! Проверяет является ли тип хранящийся в ячейке пустым списком
bool LispCell::isEmpty() {
    return type == T_EMPTY;
}

//! Проверяет является ли тип хранящийся в ячейке списком
bool LispCell::isList() {
    return end()->type == T_EMPTY;
}

//! Проверяет является ли тип хранящийся в ячеке последовательностью пар
bool LispCell::isPairList() {
    return type == T_PAIR and end()->type != T_EMPTY;
}
