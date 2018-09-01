#include "eval.hpp"
#include "exceptions.hpp"
#include "lispTypes.hpp"
#include "repl.hpp"
#include "tools.hpp"

#include <sstream>



/*!
 * \brief Вычисляет переданный ей список
 * \param exp Список выражений на вычисление который возращает функция parse
 * \param env Окружение в котором должно проходить вычисление
 */
void eval(obj_ptr exp, env_ptr& env) {
    for (; exp->type == T_PAIR; exp = exp->pair->cdr) {
        evalExpression(exp->pair->car, env);
    }
}

/*!
 * \brief Вычисляет переданное ей выражение
 * \param exp Выражение для вычисления
 * \param env Окружение в котором должно проходить вычисление
 */
obj_ptr evalExpression(obj_ptr exp, env_ptr& env) {
    if (exp->isAtom()) {
        if (exp->isSelfEvaluating()) return exp;
        else {
            auto proc = env->find(exp);
            assert(!proc.is_null(), "Unbounded variable", exp);
            return proc;
        }
    } else {
        assert(exp->isList(), "Exp is not list"); //debug

        obj_ptr& head = exp->pair->car;
        obj_ptr& tail = exp->pair->cdr;
        size_t tail_len = tail->len() - 1;
        if (head->type == T_SYMBOL) {
            Symbol& symbol = *head->symbol;

            if (symbol == "quote") {
                assertSyntax(tail_len == 1, "quote", exp);
                return tail->pair->car;
            }

            if (symbol == "if") {
                assertSyntax(tail_len == 3 or tail_len == 2, "if", exp);
                bool p = evalExpression(tail->at(0), env)->isTrue();
                if (p) return evalExpression(tail->at(1), env);
                else if (tail_len == 3) return evalExpression(tail->at(2), env);
                else return undefined();
            }

            if (symbol == "begin") {
                assertSyntax(tail_len > 0, "begin", exp);
                auto cur_env = Environment::append(env);
                return evalList(tail, cur_env);
            }

            if (symbol == "lambda") {
                assertSyntax(tail_len >= 2, "lambda", exp);
                obj_ptr lambda = makeObject(T_PROC, Procedure());
                lambda->proc->body = tail->pair->cdr;
                auto args = tail->at(0);
                assertSyntax(checkListType(T_SYMBOL, args), "lambda", exp);
                lambda->proc->formalArgs = new std::vector<obj_ptr>;
                for (; args->type != T_EMPTY; args = args->pair->cdr) {
                    lambda->proc->formalArgs->push_back(args->pair->car);
                }
                lambda->proc->maxArgsc = lambda->proc->formalArgs->size();
                if (!env->outer.is_null()) lambda->proc->environment = env;
                else lambda->proc->environment = env_ptr();
                return lambda;
            }

            if (symbol == "define") {
                obj_ptr name;

                assertSyntax(tail_len >= 1, "begin", exp);
                if (tail->pair->car->type == T_SYMBOL) {
                    name = tail->pair->car;
                    if (tail_len == 1) {
                        env->define(name, undefined());
                    } else {
                        auto body = evalExpression(tail->pair->cdr->pair->car, env);
                        env->define(name, body);
                    }
                } else if (tail->pair->car->type == T_PAIR) {
                    name = tail->pair->car->pair->car;
                    auto args = tail->pair->car->pair->cdr;
                    auto body = tail->pair->cdr;
                    assert(body->type != T_EMPTY, "define/t2", exp);
                    obj_ptr lambda = singletonList(makeObject(T_SYMBOL, Symbol("lambda")));
                    lambda->append(args);
                    for (; body->type != T_EMPTY; body = body->pair->cdr) {
                        lambda->append(body->pair->car);
                    }
                    env->define(name, evalExpression(lambda, env));
                }

                return undefined();
            }

            if (symbol == "set!") {
                assertSyntax(tail_len == 2, "set!", exp);
                auto sym = tail->pair->car;
                auto val = tail->pair->cdr->pair->car;

                bool p = env->change(sym, evalExpression(val, env));
                assert(p, "Unbounded variable", sym);
                return undefined();
            }

            if (symbol == "delay") {
                assertSyntax(tail_len == 1, "delay", exp);
                auto lambda = makeObject(T_PROC, Procedure());
                lambda->proc->body = tail;
                lambda->proc->environment = env_ptr();
                return lambda;
            }
        }

        auto procedure = evalExpression(head, env);
        assert(procedure->type == T_PROC, "bad type for apply", procedure);
        obj_ptr new_list;

        for (auto ptr = tail; ptr->type != T_EMPTY; ptr = ptr->pair->cdr) {
            if (new_list.is_null()) new_list = singletonList(evalExpression(ptr->pair->car, env));
            else new_list->append(evalExpression(ptr->pair->car, env));
        }
        if (new_list.is_null()) new_list = emptyList();

        try {
            Procedure::checkArgsCount(procedure, new_list);
            return procedure->proc->apply(new_list, env);
        } catch (LispException &e) {
            if (e.addProc) {
                e.addProc = false;
                e.errorString = "In procedure " + objectAsString(head) + ": " + e.errorString;
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
obj_ptr evalList(obj_ptr exp, env_ptr& env) {
    obj_ptr result;
    for (auto list = exp;
         list->type != T_EMPTY;
         list = list->pair->cdr) {
        result = evalExpression(list->pair->car, env);
    }
    return result;
}

