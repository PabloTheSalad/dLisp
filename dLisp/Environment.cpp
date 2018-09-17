#include "types/Environment.hpp"
#include "lib/base.hpp"
#include <iostream>

/*!
 * \brief Функция поиска символа в окружении
 * \param symbol Ссылка на искомый символ
 * \return Либо ссылку на объект - значение символа, либо указатель на пустой
 * объект
 */
obj_ptr Environment::find(obj_ptr symbol) {
    auto result = symbols->find(symbol);
    if (result != symbols->end()) {
        return result->second;
    }
    else if (!outer.isValid()) return obj_ptr();
    else return outer->find(symbol);
}

/*!
 * \brief Создает новый символ в окружении
 * \param symbol Символ для определения
 * \param exp Значение символа
 */
void Environment::define(obj_ptr symbol, obj_ptr exp) {
    auto res = symbols->emplace(symbol, exp);
    if (!res.second) symbols->at(symbol) = exp;
}

/*!
 * \brief Изменяет значение уже существующего символа
 * \param symbol Символ значение которого необходимо изменить
 * \param exp Новое значение символа
 * \return true, если значение символа было удачно изменено иначе false
 */
bool Environment::change(obj_ptr symbol, obj_ptr exp) {
    if (find(symbol).isValid()) {
        define(symbol, exp);
        return true;
    } else return false;
}

/*!
 * \brief Создает объект глобального окружения
 * \return Новый объект глобального окружения
 * 
 * Создает и возвращает новый объект глобального окружения к которому подключены
 * некоторые базовые библиотеки
 */
env_ptr makeGlobalEnv() {
    env_ptr env = makeEnv(Environment());
    env->addSymbols(Base::arithmeticFuncTable());
    env->addSymbols(Base::predicateFuncTable());
    env->addSymbols(Base::pairlistFuncTable());
    env->addSymbols(Base::exceptionFuncTable());
    env->addSymbols(Base::simpleIoFuncTable());
    return env;
}

/*!
 * \brief Добавляет символы из таблицы символов
 * 
 * Таблицы символов используются для описания интерфейса библиотек
 */
void Environment::addSymbols(const FuncTable& funcs) {
//    Function func(nullptr);
//    size_t min(0), max(0);
    for (auto f : funcs) {
        auto [func, min, max] = f.second;
        auto symbol = makeObject(T_SYMBOL, Symbol(f.first));
        auto procedure = makeObject(T_PROC, Procedure(func, min, max, symbol));
        symbols->emplace(std::move(symbol), std::move(procedure));
    }
}
