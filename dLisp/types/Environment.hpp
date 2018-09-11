/*! 
 * \file
 * \author Павел Коваленко
 * \date 23 июля 2018 г., 22:26
 */

#ifndef ENVIRONMENT_T_HPP
#define ENVIRONMENT_T_HPP

#include "Dict.hpp"
#include "types/Procedure.hpp"
#include "../lib/FuncTable.hpp"

/*!
 * \brief Класс lisp-типа окружения
 * 
 * Данный класс отвечает за представление окружений, которые хранят таблицы
 * символов связанных с значениями (процедурами)
 */
class Environment {
    Dict* symbols = nullptr;

    friend class LispCell;
public:
    env_ptr outer;
    Environment () : outer() { symbols = new Dict; }
    Environment (const Environment& env) = delete;
    Environment& operator = (const Environment& env) = delete;
    Environment& operator = (Environment&& env) = default;
    Environment (Environment&&) = default;

    static inline env_ptr append (const env_ptr& other) {
        env_ptr env = makeEnv(Environment());
        env->outer = other;
        return env;
    }
    
    inline bool operator == (const Environment& other) {
        return symbols == other.symbols and outer == other.outer;
    }
    
    Dict* getSymbols() { return symbols;}
    void addSymbols(const FuncTable&);
    obj_ptr find(obj_ptr);
    void define(obj_ptr, obj_ptr);
    bool change(obj_ptr, obj_ptr);
};

env_ptr makeGlobalEnv();

#endif /* ENVIRONMENT_T_HPP */

