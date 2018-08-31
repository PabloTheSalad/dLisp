#include "repl.hpp"
#include "eval.hpp"
#include "parse.hpp"
#include "tokenizer.hpp"
#include "exceptions.hpp"
#include "lispTypes.hpp"
#include <vector>
#include <iostream>
#include <sstream>
#include <chrono>
#include <fstream>
#include <iomanip>

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
    auto mm = initializeMemoryManager(); // инициализация менеджера памяти (обязательна для работы интерпретатора)
    auto env = makeGlobalEnv(); // создание глобального окружения
    char* str = new char[1024];
    loadFile("funcs.dlisp", env);
    while (true) {
        std::cout << ">> ";
        std::cin.getline(str, 1024);
        
        if (str[0] == ';') {
            str = str + 1;
            if (strcmp(str, "exit") == 0) {
                std::cout << "Exit repl";
                return;
            }

            if (strcmp(str, "env") == 0) {
                std::cout << "GENV: " << objAsStr(env.as_type<obj_ptr>()) << std::endl;
            }
            
            if (strncmp(str, "time ", 5) == 0) {
                str = str + 5;
                clock::time_point start, stop;
                auto tokens = tokenizer(str);
                if (!tokens.empty()) {
                    auto parsed = parse(tokens);
                    obj_ptr res;
                    for (; parsed->type != T_EMPTY; parsed = parsed->pair->cdr) {
                        start = clock::now();
                        res = evalExpression(parsed->pair->car, env);
                        stop = clock::now();
                        std::cout << "time in microseconds " 
                              << std::chrono::duration_cast<std::chrono::microseconds>(stop-start).count()
                              << std::endl;
                        std::cout << "Result: " << objAsStr(res) << std::endl;
                    }
                }
            }
            
            if (strcmp(str, "memory") == 0) {
                auto nFreeCells = mm->getFreeCellsCount();
                auto nAllocatedCells = mm->getAllocatedBlocksCount()*BLOCK_SIZE;
                std::cout << "Memory use: " << nAllocatedCells << '/'
                                            << nAllocatedCells - nFreeCells << '/'
                                            << nFreeCells
                                            << " allocated/used/free cells" << std::endl;
            }

            if (strcmp(str, "pmem") == 0) {
                auto block = mm->getMemBlocks()[0];
                auto it = block->begin();
                for (int i = 0; i < 20; i++) {
                    for (int j = 0; j < 6; j++) {
                        LispCell& obj = *(it + i+(j*20));
                        std::cout << std::setw(3) << i+(j*20) << ":"
                                  << std::setw(3) << obj.refCounter << ":" << obj.type << "| ";
                    }
                    std::cout << std::endl;
                }
            }

            if (strcmp(str, "collect") == 0) {
                mm->collectGarbageDeep();
            }

            if (strncmp(str, "getobj ", 7) == 0) {
                str += 7;
                auto idx = atoi(str);
                std::cout << "object on index " << idx << " is "
                          << objAsStr(obj_ptr(idx)) << std::endl;
            }
            
            
        } else {
            try {
                auto tokens = tokenizer(str);
                if (!tokens.empty()) {
                    auto parsed = parse(tokens);
                    for (; parsed->type != T_EMPTY; parsed = parsed->pair->cdr) {
                        std::cout << "Result: " + objAsStr(evalExpression(parsed->pair->car, env)) << std::endl;
                    }
                }
            } catch (const LispException &e) {
                if (e.isCritical) throw e;
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
std::string objAsStr(obj_ptr obj, bool in_list) {
    std::ostringstream result;
    if (obj->type == T_PAIR) {
        if (!in_list) result << "(";
        result << objAsStr(obj->pair->car, false);
        if (obj->pair->cdr->type == T_PAIR)
            result << " " << objAsStr(obj->pair->cdr, true);
        else if (obj->pair->cdr->type == T_EMPTY)
            result << ")";
        else
            result << " . " << objAsStr(obj->pair->cdr) << ")";
    } else {
        switch (obj->type) {
            case T_EMPTY:
                result << "()";
                break;
            case T_BOOL:
                if (obj->boolean) result << "#t";
                else result << "#f";
                break;
            case T_NUMBER:
                if (obj->number.type == T_INT) result << static_cast<long long>(obj->number.value);
                else result << obj->number.value;
                break;
            case T_STRING:
                result << '"' << *obj->string << '"';
                break;
            case T_SYMBOL:
                result << *obj->symbol;
                break;
            case T_PROC: {
                result << "#<procedure (";
                if (!obj->proc->function) {
                    std::vector<obj_ptr>* args = obj->proc->formalArgs;
                    if (!args->empty()) {
                        for (auto it = args->begin(); it < args->end() - 1; it++) result << *(*it)->symbol << " ";
                        result << *args->back()->symbol;
                    }
                } else {
                    bool p = obj->proc->minArgsc != obj->proc->maxArgsc;
                    if (p) result << "#:optional";
                    if (obj->proc->minArgsc != 0) {
                        if (p) result << " ";
                        for (size_t i = 0; i < obj->proc->minArgsc - 1; i++) result << "_ ";
                        result << "_";
                    }
                    if (p) result << " . _";
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
                for (auto p : *obj->env->getSymbols()) {
                    result << objAsStr(p.first) << ":"
                              << objAsStr(p.second) << ", ";
                }
                result << "} " << std::endl;
        }
    }
    return result.str();
}

void loadFile(const char* filename, env_ptr env) {
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
