#include "eval.hpp"
#include "exceptions.hpp"
#include <iostream>
#include "lisp_types.hpp"
#include "repl.hpp"

#include <sstream>

#define undefined() make_object(T_SPECIAL, special_t(UNDEF))

/*!
 * \brief Вычисляет переданный ей список
 * \param exp Список выражений на вычисление который возращает функция parse
 * \param env Окружение в котором должно проходить вычисление
 */
void eval(obj_ptr exp, env_ptr& env) {
    for (; exp->type == T_PAIR; exp = exp->pair->cdr) {
        eval_exp(exp->pair->car, env);
    }
}

/*!
 * \brief Вычисляет переданное ей выражение
 * \param exp Выражение для вычисления
 * \param env Окружение в котором должно проходить вычисление
 */
obj_ptr eval_exp(obj_ptr exp, env_ptr& env) {
    if (exp->is_atom()) {
        if (exp->is_self_evaluating()) return exp;
        else {
            auto proc = env->find(exp);
            assert(!proc.is_null(), "Unbounded variable", exp);
            return proc;
        }
    } else {
        assert(exp->is_list(), "Exp is not list"); //debug

        obj_ptr& head = exp->pair->car;
        obj_ptr& tail = exp->pair->cdr;
        size_t tail_len = tail->len() - 1;
        if (head->type == T_SYMBOL) {
            symbol_t& symbol = *head->symbol_ptr;

            if (symbol == "quote") {
                assert_syntax(tail_len == 1, "quote", exp);
                return tail->pair->car;
            }

            if (symbol == "if") {
                assert_syntax(tail_len == 3 or tail_len == 2, "if", exp);
                bool p = eval_exp(tail->at(0), env)->is_true();
                if (p) return eval_exp(tail->at(1), env);
                else if (tail_len == 3) return eval_exp(tail->at(2), env);
                else return undefined();
            }

            if (symbol == "begin") {
                assert_syntax(tail_len > 0, "begin", exp);
                auto cur_env = environment_t::append(env);
                return evlis(tail, cur_env);
            }

            if (symbol == "lambda") {
                assert_syntax(tail_len >= 2, "lambda", exp);
                obj_ptr lambda = make_object(T_PROC, procedure_t());
                lambda->proc->body = tail->pair->cdr;
                auto args = tail->at(0);
                assert_syntax(check_list_type(T_SYMBOL, args), "lambda", exp);
                lambda->proc->formal_args = new std::vector<obj_ptr>;
                for (; args->type != T_NULL; args = args->pair->cdr) {
                    lambda->proc->formal_args->push_back(args->pair->car);
                }
                if (!env->outer.is_null()) lambda->proc->environment = env;
                else lambda->proc->environment = env_ptr();
                return lambda;
            }

            if (symbol == "define") {
                obj_ptr name;

                assert_syntax(tail_len >= 1, "begin", exp);
                if (tail->pair->car->type == T_SYMBOL) {
                    name = tail->pair->car;
                    if (tail_len == 1) {
                        env->define(name, undefined());
                    } else {
                        auto body = eval_exp(tail->pair->cdr->pair->car, env);
                        env->define(name, body);
                    }
                } else if (tail->pair->car->type == T_PAIR) {
                    name = tail->pair->car->pair->car;
                    auto args = tail->pair->car->pair->cdr;
                    auto body = tail->pair->cdr;
                    assert(body->type != T_NULL, "define/t2", exp);
                    obj_ptr lambda = singleton_list(make_object(T_SYMBOL, symbol_t("lambda")));
                    lambda->append(args);
                    for (; body->type != T_NULL; body = body->pair->cdr) {
                        lambda->append(body->pair->car);
                    }
                    env->define(name, eval_exp(lambda, env));
                }

                return undefined();
            }

            if (symbol == "set!") {
                assert_syntax(tail_len == 2, "set!", exp);
                auto sym = tail->pair->car;
                auto val = tail->pair->cdr->pair->car;

                bool p = env->change(sym, eval_exp(val, env));
                assert(p, "Unbounded variable", sym);
                return undefined();
            }

            if (symbol == "delay") {
                assert_syntax(tail_len == 1, "delay", exp);
                auto lambda = make_object(T_PROC, procedure_t());
                lambda->proc->body = tail;
                lambda->proc->environment = env_ptr();
                return lambda;
            }
        }

        auto form = eval_exp(head, env);
        assert(form->type == T_PROC, "bad type for apply", form);
        obj_ptr new_list;
        for (auto ptr = tail; ptr->type != T_NULL; ptr = ptr->pair->cdr) {
            if (new_list.is_null()) new_list = singleton_list(eval_exp(ptr->pair->car, env));
            else new_list->append(eval_exp(ptr->pair->car, env));
        }
        if (new_list.is_null()) new_list = empty_list();

        bool p;
        if (form->proc->function) {
            p = form->proc->argsc == new_list->len() - 1;
        } else p = form->proc->formal_args->size() == new_list->len() - 1;

        if (!p) {
            lisp_error err("In procedure ", form);
            std::ostringstream err_str;
            err_str << obj_as_str(head) << ": "
                    << "Wrong number of args to "
                    << obj_as_str(form) << ": " << new_list->len()-1
                    << " expected "
                    << (form->proc->function? form->proc->argsc: form->proc->formal_args->size());
            err.err_str += err_str.str();
            throw err;
        }
        
        try {
            return form->proc->apply(new_list, env);
        } catch (lisp_error &e) {
            if (e.add_proc and !e.critical) {
                e.add_proc = false;
                e.err_str = "In procedure " + obj_as_str(head) + ": " + e.err_str;
                throw e;
            }
            throw e;
        }
    }
}

/*!
 * \brief Вычисляет каждый элемент списка и возвращает значение последнего
 * \param exp Список для вычисления
 * \parma env Окружение в котором должно проходить вычисление
 */
obj_ptr evlis(obj_ptr exp, env_ptr& env) {
    obj_ptr result;
    for (auto list = exp;
         list->type != T_NULL;
         list = list->pair->cdr) {
        result = eval_exp(list->pair->car, env);
    }
    return result;
}
