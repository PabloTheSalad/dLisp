#include "eval.hpp"
#include "exceptions.hpp"
#include "lispTypes.hpp"
#include "repl.hpp"
#include "tools.hpp"
#include "specialForms.h"
#include <sstream>



/*!
 * \brief Вычисляет переданный ей список
 * \param exp Список выражений на вычисление который возращает функция parse
 * \param env Окружение в котором должно проходить вычисление
 */
void eval(obj_ptr exp, env_ptr& env) {
    for (; exp->type == T_PAIR; exp = exp->pair().cdr) {
        evalExpression(exp->pair().car, env);
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
            assert(proc.isValid(), "Unbounded variable", exp);
            return proc;
        }
    } else {
        //assert(exp->isList(), "Exp is not list"); //debug

        obj_ptr& head = exp->pair().car;
        obj_ptr& tail = exp->pair().cdr;
        size_t tailLen = tail->len() - 1;
        if (head->type == T_SYMBOL) {
            Symbol& symbol = head->symbol();

            if (symbol == "quote") {
                assertSyntax(tailLen == 1, "quote", exp);
                return tail->pair().car;
            }

            if (symbol == "if") {
                assertSyntax(tailLen == 3 or tailLen == 2, "if", exp);
                bool p = evalExpression(tail->at(0), env)->isTrue();
                if (p) return evalExpression(tail->at(1), env);
                else if (tailLen == 3) return evalExpression(tail->at(2), env);
                else return unspecified();
            }

            if (symbol == "begin") {
                assertSyntax(tailLen > 0, "begin", exp);
                auto cur_env = Environment::append(env);
                return evalList(tail, cur_env);
            }

            if (symbol == "lambda") {
                assertSyntax(tailLen >= 2, "lambda", exp);
                obj_ptr lambda = makeObject(T_PROC, Procedure());
                lambda->procedure().body = tail->pair().cdr;
                auto args = tail->at(0);
                assertSyntax(checkListType(T_SYMBOL, args), "lambda", exp);
                lambda->procedure().formalArgs = new std::vector<obj_ptr>;
                for (; args->type != T_EMPTY; args = args->pair().cdr) {
                    lambda->procedure().formalArgs->push_back(args->pair().car);
                }
                lambda->procedure().maxArgsc = lambda->procedure().formalArgs->size();
                if (env->outer.isValid()) lambda->procedure().environment = env;
                else lambda->procedure().environment = env_ptr();
                return lambda;
            }

            if (symbol == "define") {
                obj_ptr name;

                assertSyntax(tailLen >= 1, "begin", exp);
                if (tail->pair().car->type == T_SYMBOL) {
                    name = tail->pair().car;
                    if (tailLen == 1) {
                        env->define(name, unspecified());
                    } else {
                        auto body = evalExpression(tail->pair().cdr->pair().car, env);
                        env->define(name, body);
                    }
                } else if (tail->pair().car->type == T_PAIR) {
                    name = tail->pair().car->pair().car;
                    auto args = tail->pair().car->pair().cdr;
                    auto body = tail->pair().cdr;
                    assertSyntax(body->type != T_EMPTY, "define/t2", exp);
                    obj_ptr lambda = singletonList(makeObject(T_SYMBOL, Symbol("lambda")));
                    lambda->append(args);
                    for (; body->type != T_EMPTY; body = body->pair().cdr) {
                        lambda->append(body->pair().car);
                    }
                    auto procedure = evalExpression(lambda, env);
                    procedure->procedure().procName = name;
                    env->define(name, procedure);
                }

                return unspecified();
            }

            if (symbol == "set!") {
                assertSyntax(tailLen == 2, "set!", exp);
                auto sym = tail->pair().car;
                auto val = tail->pair().cdr->pair().car;

                bool p = env->change(sym, evalExpression(val, env));
                assert(p, "Unbounded variable", sym);
                return unspecified();
            }

            if (symbol == "delay") {
                assertSyntax(tailLen == 1, "delay", exp);
                auto lambda = makeObject(T_PROC, Procedure());
                lambda->procedure().body = tail;
                lambda->procedure().environment = env_ptr();
                return lambda;
            }

            if (symbol == "let") {
                assertSyntax(tailLen >= 2
                             and (tail->at(0)->type == T_PAIR
                                  or tail->at(0)->type == T_EMPTY), "let", exp);
                obj_ptr argsList = tail->at(0);
                forAllInList(argsList, [&](auto pair){
                    assertSyntax(pair->len()-1 == 2
                                 and pair->type == T_PAIR
                                 and pair->at(0)->type == T_SYMBOL, "let", exp);
                });
                return evalExpression(letMacro(tail), env);
            }

            if (symbol == "environment") {
                assertSyntax(tailLen == 0, "environment", exp);
                auto new_env = makeEnv(Environment());
                new_env->outer = env;
                return new_env.as_type<obj_ptr>();
            }

            if (symbol == "eval") {
                assertSyntax(tailLen == 1 or tailLen == 2, "eval", exp);
                env_ptr& cur_env = env;
                if (tailLen == 2) {
                    auto obj = evalExpression(tail->at(1), env);
                    assertSyntax(obj->type = T_ENV, "eval", exp);
                    cur_env = obj.as_type<env_ptr>();
                }
                return evalExpression(evalExpression(tail->at(0), env), cur_env);
            }

            if (symbol == "while") {
                assertSyntax(tailLen >= 2, "while", exp);
                auto body = tail->pair().cdr;
                while (evalExpression(tail->at(0), env)->isTrue()) {
                    evalList(body, env);
                }
                return unspecified();
            }
        }

        auto procedure = evalExpression(head, env);
        assert(procedure->type == T_PROC, "bad type for apply", procedure);
        obj_ptr new_list;

        for (auto ptr = tail; ptr->type != T_EMPTY; ptr = ptr->pair().cdr) {
            if (!new_list.isValid()) new_list = singletonList(evalExpression(ptr->pair().car, env));
            else new_list->append(evalExpression(ptr->pair().car, env));
        }
        if (!new_list.isValid()) new_list = emptyList();

        try {
            Procedure::checkArgsCount(procedure, new_list);
            return procedure->procedure().apply(new_list, env);
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
 * \param env Окружение в котором должно проходить вычисление
 */
obj_ptr evalList(obj_ptr exp, env_ptr& env) {
    obj_ptr result;
    forAllInList(exp, [&env, &result](auto obj){
        result = evalExpression(obj, env);
    });
    return result;
}

