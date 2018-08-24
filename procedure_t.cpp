#include "exceptions.hpp"
#include "eval.hpp"
#include "repl.hpp"
#include "lisp_types.hpp"

procedure_t& procedure_t::operator = (const procedure_t& other) {
    function = other.function;
    argsc = other.argsc;
    body = other.body;
    formal_args = other.formal_args;
    environment = other.environment;
    return *this;
}

procedure_t::procedure_t(const procedure_t& other) {
    *this = other;
}

bool procedure_t::operator == (const procedure_t& other) {
    return function == other.function and
           argsc == other.argsc and
           body == other.body and
           formal_args == other.formal_args and
           environment == other.environment;
}

env_ptr append(env_ptr env, env_ptr other) {
    env_ptr env_tmp = env;
    for (; !env_tmp->outer.is_null(); env_tmp = env_tmp->outer);
    env_ptr env_o = other;
    bool cycle = false;
    for (; !env_o.is_null(); env_o = env_o->outer) {
        if (env_o == env) {
            cycle = true;
            break;
        }
    }
    if (!cycle) env_tmp->outer = other;
    return env_tmp;
}

obj_ptr procedure_t::apply(obj_ptr args, env_ptr& env) {
    if (function) return function(args, env);

    env_ptr tail;
    if (environment.is_null()) environment = env;
    else tail = append(environment, env);
    
    obj_ptr result;
    auto args_env = environment_t::append(environment);
    if (args->type != T_NULL) {
        int i = 0;
        for(obj_ptr list = args; list->type != T_NULL; list = list->pair->cdr, i++) {
            args_env->define(formal_args->at(i), list->pair->car);
        }
        //args_env->define(formal_args[i]->symbol, list->pair.car);
        result = evlis(body, args_env);
    } else result = evlis(body, args_env);
    
    if (tail.is_null()) environment = env_ptr();
    else tail->outer = env_ptr();
    
    return result;
}
