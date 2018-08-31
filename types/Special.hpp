/*! 
 * \file
 * \author Павел Коваленко
 * \date 28 июля 2018 г., 17:59
 */

#ifndef SPECIAL_T_HPP
#define SPECIAL_T_HPP

//! Флаги типов специальных объектов
enum SpecialTypeFlag : char {
    UNDEF, ///< Неопределенное значение
    INF, ///< Бесконечность
    NAN ///< Нечисловое значение (NaN - Not a Number)
};

/*!
 * \brief Класс lisp-типа специальных значение
 * 
 * Данный класс предназначен для предсталения специальных внутрених значений интерпретатора,
 * которые не могут быть использованы в вычислениях напрямую
 */
struct Special {
    SpecialTypeFlag type;
    Special(SpecialTypeFlag type) : type(type) {}
    Special(const Special&);
    inline bool operator == (const Special& other) { return type == other.type; }
};

////! Создает объект - неопределенное значение
//inline obj_ptr undefined() {
//    return make_object(T_SPECIAL, special_t(UNDEF));
//}

#endif /* SPECIAL_T_HPP */

