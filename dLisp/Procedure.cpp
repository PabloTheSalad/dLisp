#include "exceptions.hpp"
#include "eval.hpp"
#include "repl.hpp"
#include "lispTypes.hpp"
#include <sstream>

bool Procedure::operator == (const Procedure& other) {
    return function == other.function and
           minArgsc == other.minArgsc and
           maxArgsc == other.maxArgsc and
           body == other.body and
           formalArgs == other.formalArgs and
           environment == other.environment;
}

/*!
 * \brief ДДелает окружение env внешним для окружения other,
 * но только если не обноружено циклов окружений
 * \param env Окружение которое должно быть внешним
 * \param other Окружение к которому окружение env должно стать внешним
 * \return Возвращает окружение в котором произошло соединение окружений env и other
 */
env_ptr append(env_ptr env, env_ptr other) {
    env_ptr env_tmp = env;
    env_ptr env_o = other;
    bool cycle = false;

    for (; env_tmp.isValid(); env_tmp = env_tmp->outer) {
        if (env_tmp == other) {
            cycle = true;
            break;
        }
    }

    for (; env_o.isValid() and !cycle; env_o = env_o->outer) {
        if (env_o == env) {
            cycle = true;
            break;
        }
    }

    for (; !cycle && env_tmp->outer.isValid(); env_tmp = env_tmp->outer);

    if (!cycle) env_tmp->outer = other;
    return env_tmp;
}

/*!
 * \brief Применяет аргументы к процедуре в переданном окружении
 * \param args Аргументы для процедуры
 * \param env Окружение в котором должна быть вычислена процедура
 * \return Результат вычисления процедуры
 */
obj_ptr Procedure::apply(obj_ptr args, env_ptr& env) {
    if (function) return function(args);

    env_ptr tail;
    if (!environment.isValid()) environment = env;
    else tail = append(environment, env);
    
    obj_ptr result;
    auto args_env = Environment::append(environment);
    if (args->type != T_EMPTY) {
        size_t i = 0;
        for(obj_ptr list = args; list->type != T_EMPTY; list = list->pair().cdr, i++) {
            args_env->define(formalArgs->at(i), list->pair().car);
        }
        result = evalList(body, args_env);
    } else result = evalList(body, args_env);
    
    if (!tail.isValid()) environment = env_ptr();
    else tail->outer = env_ptr();
    
    return result;
}

/*!
 * \brief Проверяет соответсвует ли количество аргументов передаваемых процедуре
 *  количеству аргументов которые процедура способна принять
 * \param proc Процедура для проверки
 * \param args Список аргументов для проверки
 */
void Procedure::checkArgsCount(obj_ptr proc, obj_ptr args) {
    Procedure& procedure = proc->procedure();
    bool p;
    size_t min_argsc;
    size_t max_argsc;
    size_t real_argsc = args->len() - 1;
    if (procedure.function) {
        min_argsc = procedure.minArgsc;
    } else min_argsc = procedure.formalArgs->size();
    max_argsc = procedure.maxArgsc;

    if (max_argsc < min_argsc and real_argsc >= min_argsc) p = true;
    else if (min_argsc <= real_argsc and real_argsc <= max_argsc) p = true;
    else p = false;

    if (!p) {
        LispException err("");
        err.addProc = true;
        std::ostringstream err_str;
        err_str << "Wrong number of args to "
                << objectAsString(proc) << ": " << args->len()-1
                << " expected "
                << (procedure.function? procedure.minArgsc: procedure.formalArgs->size());
        err.errorString += err_str.str();
        throw err;
    }
}
