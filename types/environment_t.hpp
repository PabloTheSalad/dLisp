/*! 
 * \file
 * \author Павел Коваленко
 * \date 23 июля 2018 г., 22:26
 */

#ifndef ENVIRONMENT_T_HPP
#define ENVIRONMENT_T_HPP

#include "dict_t.hpp"
#include "types/procedure_t.hpp"

/*!
 * \brief Класс lisp-типа окружения
 * 
 * Данный класс отвечает за представление окружений, которые хранят таблицы
 * символов связанных с значениями (процедурами)
 */
class environment_t {
    dict_t* symbols;
public:
    env_ptr outer;
    environment_t () : outer() { symbols = new dict_t; }
    environment_t (const environment_t& env) : outer(env.outer), symbols(env.symbols) {}
    
    static env_ptr append (const env_ptr& other) {
        env_ptr env = make_env(environment_t());
        env->outer = other;
        return env;
    }
    
    environment_t& operator = (const environment_t& env) {
        symbols = env.symbols;
        outer = env.outer;
        return *this;
    }
    
    inline bool operator == (const environment_t& other) {
        return symbols == other.symbols and outer == other.outer;
    }
    
    dict_t* get_symbols() { return symbols;}
    void add_symbols(const std::map<const char*, std::pair<function_t, size_t>>&);
    obj_ptr find(obj_ptr);
    void define(obj_ptr, obj_ptr);
    bool change(obj_ptr, obj_ptr);
    //env_ptr append(env_ptr&);
};

env_ptr make_global_env();

#endif /* ENVIRONMENT_T_HPP */

