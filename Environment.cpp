#include "types/Environment.hpp"
#include "lib/base.hpp"

/*!
 * \brief Функция поиска символа в окружении
 * \param symbol Ссылка на искомый символ
 * \return Либо ссылку на объект - значение символа, либо указатель на пустой
 * объект
 */
obj_ptr Environment::find(obj_ptr symbol) {
    auto result = symbols->find(symbol);
    if (result != symbols->end()) {
        //result->second->refCounter--; //STL-контейнеры не конструируют объект внутри себя...
        return result->second;
    }
    else if (outer.is_null()) return obj_ptr();
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
    if (!find(symbol).is_null()) {
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
    auto env = makeEnv(Environment());
    env->outer = env_ptr();
    env->addSymbols(Base::Predicate::func_table);
    env->addSymbols(Base::Arithmetic::funcTable);
    env->addSymbols(Base::Pairlist::funcTable);
    return env;
}

/*
 * \brief Прицеляет окружение в конец переданной цепочки окружений
 * \param other Окружение которое необходимо прицепить
 * \return Указатель на окружение к которому было прицеплено окружение \a other
 * 
env_ptr environment_t::append(env_ptr& other) {
    env_ptr env = get_env(this);
    for (; !env->outer.is_null(); env = env->outer);
    env_ptr env_o = other;
    bool cycle = false;
    for (; !env_o.is_null(); env_o = env_o->outer) {
        if (env_o == get_env(this)) {
            cycle = true;
            break;
        }
    }
    if (!cycle) env->outer = other;
    return env;
}
*/

/*!
 * \brief Добавляет символы из таблицы символов
 * 
 * Таблицы символов используются для описания интерфейса библиотек
 */
void Environment::addSymbols(const FuncTable& funcs) {
    Function func;
    size_t min;
    size_t max;
    for (auto f: funcs) {
        std::tie(func, min, max) = f.second;
        symbols->emplace(makeObject(T_SYMBOL, Symbol(f.first)),
                        makeObject(T_PROC, Procedure(func, min, max)));
    }
}
