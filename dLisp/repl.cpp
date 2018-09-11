#include "repl.hpp"
#include "eval.hpp"
#include "parse.hpp"
#include "tokenizer.hpp"
#include "exceptions.hpp"
#include "lispTypes.hpp"
#include "tools.hpp"
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <chrono>

/*!
 * \brief Вызывает консольный REPL-рeжим интерпретатора
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
 * timeit [выражение] - вычисляет [выражение] в цикле 100 раз и вычисляет среднее время
 * memory - вывод состояния памяти
 */
void repl(MemoryManager* memoryManager, env_ptr env) {
    char* command = new char[1024];
    std::cout << "Enter ';help' for help." << std::endl;
    while (true) {
        std::cout << ">> ";

        if (std::cin.get() == ';') {
            std::cin.getline(command, 1024);
            auto mutcmd = command;

            if (strcmp(mutcmd, "help") == 0) {
                std::cout << "Доступные команды" << std::endl << std::endl
                          << "  ;help               - показать эту справку" << std::endl
                          << "  ;exit               - завершить работу интерпретатора" << std::endl
                          << "  ;env                - вывести содержание глобального окружения" << std::endl
                          << "  ;time <выражение>   - измерить время вычисления <выражения>" << std::endl
                          << "  ;timeit <выражение> - измеряет среднее время вычисления" << std::endl
                          << "                               <выражения> для 100 циклов" << std::endl
                          << "  ;memory             - вывести потребление памяти" << std::endl
                          << std::endl;
            }

            if (strcmp(mutcmd, "exit") == 0) {
                std::cout << "Exit repl" << std::endl;
                delete[] command;
                return;
            }

            if (strcmp(mutcmd, "env") == 0) {
                std::cout << "GENV: "
                          << objectAsString(env.as_type<obj_ptr>())
                          << std::endl;
            }
            
            if (strncmp(mutcmd, "time ", 5) == 0) {
                mutcmd += 5;
                auto parsedCode = tokenizeAndParseForm(std::cin);
                if (parsedCode->type != T_EMPTY) {
                    forAllInList(parsedCode, [&env](auto exp){
                        obj_ptr res;
                        std::chrono::microseconds time;
                        std::tie(time, res) = measureEvalExpTime(exp, env);
                        std::cout << "time in microseconds "
                                  << time.count() << std::endl
                                  << "Result: " << objectAsString(res)
                                  << std::endl;
                    });
                }
            }

            if (strncmp(mutcmd, "timeit ", 7) == 0) {
                using namespace std::chrono_literals;
                mutcmd += 7;
                auto parsedCode = tokenizeAndParseForm(std::cin);
                if (parsedCode->type != T_EMPTY) {
                    forAllInList(parsedCode, [&env](auto exp){
                       std::chrono::microseconds allTime(0), time(0),
                               minTime(0), maxTime(0);
                       obj_ptr result;
                       for (int i = 0; i < 100; i++) {
                           std::tie(time, result) = measureEvalExpTime(exp, env);
                           allTime += time;
                           if (time > maxTime) maxTime = time;
                           if (minTime == 0us) minTime = time;
                           else if (time < minTime) minTime = time;
                       }
                       std::cout << "Mean time for 100 cycles is "
                                 << (allTime/100).count() << " mcs" << std::endl
                                 << "Max time " << maxTime.count() << " mcs" << std::endl
                                 << "Min time " << minTime.count() << " mcs" << std::endl
                                 << "All time " << allTime.count() << " mcs"
                                 << std::endl
                                 << "Result:" << objectAsString(result)
                                 << std::endl;
                    });
                }
            }

            if (strncmp(mutcmd, "load ", 5) == 0) {
                mutcmd += 5;
                if(!evalFile(mutcmd, env)) {
                    std::cout << "File " << mutcmd
                              << " not loaded" << std::endl;
                } else {
                    std::cout << "File " << mutcmd
                              << " loaded in interpreter" << std::endl;
                }
            }
            
            if (strcmp(mutcmd, "memory") == 0) {
                auto nFreeCells = memoryManager->getFreeCellsCount();
                auto nAllocatedCells = memoryManager->getAllocatedBlocksCount()*BLOCK_SIZE;
                std::cout << "Memory use: "
                          << nAllocatedCells << '/'
                          << nAllocatedCells - nFreeCells << '/'
                          << nFreeCells
                          << " allocated/used/free cells" << std::endl;
            }

            if (strcmp(mutcmd, "pmem") == 0) {
                auto block = memoryManager->getMemBlocks()[0];
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

            if (strcmp(mutcmd, "collect") == 0) {
                memoryManager->collectGarbageDeep();
            }

            if (strncmp(mutcmd, "getobj ", 7) == 0) {
                mutcmd += 7;
                auto idx = atoi(mutcmd);
                std::cout << "object on index " << idx << " is "
                          << objectAsString(obj_ptr(idx)) << std::endl;
            }
            
        } else {
            std::cin.unget();
            evalAndPrintStream(std::cin, env);
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
std::string objectAsString(obj_ptr obj, bool in_list) {
 //   if (!obj.isValid()) return "";
    std::ostringstream result;
    if (obj->type == T_PAIR) {
        if (!in_list) result << "(";
        result << objectAsString(obj->pair().car, false);
        if (obj->pair().cdr->type == T_PAIR)
            result << " " << objectAsString(obj->pair().cdr, true);
        else if (obj->pair().cdr->type == T_EMPTY)
            result << ")";
        else
            result << " . " << objectAsString(obj->pair().cdr) << ")";
    } else {
        switch (obj->type) {
            case T_EMPTY:
                result << "()";
                break;
            case T_BOOL:
                if (obj->boolean()) result << "#t";
                else result << "#f";
                break;
            case T_NUMBER:
                if (obj->number().type == T_INT) result << static_cast<long long>(obj->number().value);
                else result << obj->number().value;
                break;
            case T_STRING:
                result << '"' << obj->string() << '"';
                break;
            case T_SYMBOL:
                result << obj->symbol();
                break;
            case T_PROC: {
                result << "#<procedure ";
                if (obj->procedure().procName.isValid())
                    result << objectAsString(obj->procedure().procName) << " ";
                result << "(";
                if (!obj->procedure().function) {
                    std::vector<obj_ptr>* args = obj->procedure().formalArgs;
                    if (!args->empty()) {
                        for (auto it = args->begin(); it < args->end() - 1; it++) result << (*it)->symbol() << " ";
                        result << args->back()->symbol();
                    }
                } else {
                    bool p = obj->procedure().minArgsc != obj->procedure().maxArgsc;
                    if (p) result << "#:optional";
                    if (obj->procedure().minArgsc != 0) {
                        if (p) result << " ";
                        for (size_t i = 0; i < obj->procedure().minArgsc - 1; i++) result << "_ ";
                        result << "_";
                    }
                    if (p) result << " . _";
                 }
                result << ")>";
            }
                break;
            case T_SPECIAL:
                switch(obj->special().type) {
                    case UNSPEC:
                        result << "#<unspecified>";
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
                result << "{ " << std::endl;
                for (auto p : *obj->environment().getSymbols()) {
                    result << objectAsString(p.second) << ", " << std::endl;
                }
                result << "} " << std::endl;
        }
    }
    return result.str();
}

/*!
 * \brief Выполняет файл с именем filename
 * \param filename Имя файла для выполнения
 * \param env Окружение в котором должно произойти выполнение
 * \return true если выполнение произошло успешно, false - если файл не найден
 */
bool evalFile(const char* filename, env_ptr env) {
    std::ifstream file(filename);
    if (file.is_open()) {
        while (true) {
            auto parsed = tokenizeAndParseForm(file);
            if (file.eof()) break;
            else if (!parsed.isValid()) continue;
            evalExpression(parsed, env);
        }
        return true;
    } else return false;
}

/*!
 * \brief Вычисляет код предсталенный в виде одной строки и выводит результат вычислений
 * \param code Строка кода
 * \param env Окружение в котором должно произойти вычисление
 */
void evalAndPrintStream(std::istream& in, env_ptr env) {
    try {
        obj_ptr parsedForm;
        while (true) {
            parsedForm = tokenizeAndParseForm(in);
            if (!parsedForm.isValid()) break;
            auto obj = evalExpression(parsedForm, env);
            if (obj->type != T_SPECIAL) {
                std::cout << "Result: " << objectAsString(obj) << std::endl;

            }
        }
    } catch (const LispException &e) {
        if (e.isCritical) throw e;
        std::cerr << e.what() << std::endl;
        if (std::cin.peek() == '\n') std::cin.get();
    }
}

//! Разбивает строку текста на токены, а затем парсит её,
//! возвращает объект предсталяющий переданный в функцию код
obj_ptr tokenizeAndParseForm(std::istream& in) {
    auto tokens = streamTokenizer(in);
    if (!tokens.empty()) return parseForm(tokens);
    else return obj_ptr();
}

/*!
 * \brief Выполняет выражение и подсчитывает потребовавшееся на это время
 * \param exp Выражение которое необходимо вычислить
 * \param env Окружение в котором должно произойти вычисление
 * \return Пара значений:
 * Первое значение - время в микросекундах затраченное на вычисление
 * Второе значение - результат вычисления
 */
std::pair<std::chrono::microseconds, obj_ptr>
measureEvalExpTime(obj_ptr exp, env_ptr env) {
    using hrClock = std::chrono::high_resolution_clock;
    hrClock::time_point start, stop;
    if (exp->type != T_EMPTY) {
        start = hrClock::now();
        obj_ptr res = evalExpression(exp, env);
        stop = hrClock::now();
        return std::make_pair(std::chrono::duration_cast<std::chrono::microseconds>(stop-start),
                              res);
    } else return std::make_pair(std::chrono::microseconds(0), emptyList());
}
