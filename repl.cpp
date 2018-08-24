#include "repl.hpp"
#include "eval.hpp"
#include "parse.hpp"
#include "tokenizer.hpp"
#include "exceptions.hpp"
#include "lisp_types.hpp"
#include <vector>
#include <iostream>
#include <sstream>
#include <chrono>
#include <fstream>

#define clock std::chrono::high_resolution_clock

/*!
 * \brief Вызывает консольный REPL-ркжим интерпретатора
 * 
 * В REPL-режиме команды вводятся после приглашения к вводу ">>"
 * после чего они передаются на выполнение интерпретатору, также REPL-режим
 * поддерживает ряд специфических для него команд (все команды должны начинаться
 * на "::", без пробелов)
 * 
 * Команды:
 * exit - выход из консоли
 * env - вывод содержания текущего окружения
 * time [выражение] - вычисляет [выражение] и подсчитывает потребовавшееся на это время
 * memory - вывод состояния памяти
 */
void repl() {
    auto mm = init_memory_manager(); // инициализация менеджера памяти (обязательна для работы интерпретатора)
    auto env = make_global_env(); // создание глобального окружения
    char* str = new char[1024];
    load_file("funcs.dlisp", env);
    while (true) {
        std::cout << ">> ";
        std::cin.getline(str, 1024);
        
        if (str[0] == ':' and str[1] == ':') {
            str = str + 2;
            if (strcmp(str, "exit") == 0) {
                std::cout << "Exit repl";
                return;
            }

            if (strcmp(str, "env") == 0) {
                std::cout << "GENV: " << obj_as_str(env.as_type<obj_ptr>()) << std::endl;
            }
            
            if (strncmp(str, "time ", 5) == 0) {
                str = str + 5;
                clock::time_point start, stop;
                auto tokens = tokenizer(str);
                if (!tokens.empty()) {
                    auto parsed = parse(tokens);
                    obj_ptr res;
                    for (; parsed->type != T_NULL; parsed = parsed->pair->cdr) {
                        start = clock::now();
                        res = eval_exp(parsed->pair->car, env);
                        stop = clock::now();
                        std::cout << "time in microseconds " 
                              << std::chrono::duration_cast<std::chrono::microseconds>(stop-start).count()
                              << std::endl;
                        std::cout << "Result: " << obj_as_str(res) << std::endl;
                    }
                }
            }
            
            if (strcmp(str, "memory") == 0) {
                std::cout << "Memory use: " << mm->allocated_blocks_count()*BLOCK_SIZE << '/'
                                            << mm->cells_count() << '/'
                                            << mm->free_cells_count()
                                            << " allocated/used/free cells" << std::endl;
            }
            
            if (strcmp(str, "expmem") == 0) {
                mm->expand_memory();
            }
            
        } else {
            try {
                auto tokens = tokenizer(str);
                if (!tokens.empty()) {
                    auto parsed = parse(tokens);
                    for (; parsed->type != T_NULL; parsed = parsed->pair->cdr) {
                        std::cout << "Result: " + obj_as_str(eval_exp(parsed->pair->car, env)) << std::endl;
                    }
                }
            } catch (const lisp_error &e) {
                if (e.critical) throw e;
                std::cerr << e.what() << std::endl;
            }
        }
    }
}

/*!
 * \brief Преобразует lisp-объект в его текстовое представление
 * \param obj объект который необходимо преобразовать в строку
 * \param in_list параметр определяющий является ли данный объект частью списка
 * по умолчанию false
 * \return Строку-текстовое представление \a obj
 */
std::string obj_as_str(obj_ptr obj, bool in_list) {
    std::ostringstream result;
    if (obj->type == T_PAIR) {
        if (!in_list) result << "(";
        result << obj_as_str(obj->pair->car, false);
        if (obj->pair->cdr->type != T_NULL)
            result << " " << obj_as_str(obj->pair->cdr, true);
        else result << ")";
    } else {
        switch (obj->type) {
            case T_NULL:
                result << "()";
                break;
            case T_BOOL:
                if (obj->boolean) result << "#t";
                else result << "#f";
                break;
            case T_NUMBER:
                if (obj->number.type == T_INT) result << (long long)obj->number.value;
                else result << obj->number.value;
                break;
            case T_STRING:
                result << '"' << obj->string_ptr << '"';
                break;
            case T_SYMBOL:
                result << *obj->symbol_ptr;
                break;
            case T_PROC: {
                result << "#<procedure (";
                if (!obj->proc->function) {
                    std::vector<obj_ptr>* args = obj->proc->formal_args;
                    if (!args->empty()) {
                        for (auto it = args->begin(); it < args->end() - 1; it++) result << (*it)->symbol_ptr << " ";
                        result << *args->back()->symbol_ptr;
                    }
                } else {
                    for (size_t i = 0; i < obj->proc->argsc - 1; i++) result << "_ ";
                    result << "_";
                 }
                result << ")>";
            }
                break;
            case T_SPECIAL:
                switch(obj->spec.type) {
                    case UNDEF:
                        result << "#<undefined>";
                        break;
                    case INF:
                        result << "inf";
                        break;
                    case NAN:
                        result << "NaN";
                        break;
                }
                break;
            case T_ENV:
                result << "{ ";
                for (auto p : *obj->env->get_symbols()) {
                    result << obj_as_str(p.first) << ":" 
                              << obj_as_str(p.second) << ", ";
                }
                result << "} " << std::endl;
        }
    }
    return result.str();
}

void load_file(const char* filename, env_ptr env) {
    std::ifstream file(filename);
    file.seekg(0, std::ios_base::end);
    auto size = file.tellg();
    file.seekg(0, std::ios_base::beg);
    auto code = new char[size];
    file.get(code, size, '\0');

    auto tokens = tokenizer(code);
    auto parsed = parse(tokens);
    eval(parsed, env);
}
