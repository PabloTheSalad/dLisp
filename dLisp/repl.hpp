/*!
 * \file
 * \author Павел Коваленко
 * \date 26 июля 2018 г., 1:49
 */

#ifndef REPL_HPP
#define REPL_HPP

#include <iostream>
#include "mm_ptr.hpp"
#include <chrono>

std::string objectAsString(obj_ptr, bool in_list = false);
void repl(MemoryManager*, env_ptr);
bool evalFile(const char* filename, env_ptr env);
void evalAndPrintStream(std::istream&, env_ptr);
obj_ptr tokenizeAndParseForm(std::istream& in);
std::pair<std::chrono::microseconds, obj_ptr> measureEvalExpTime(obj_ptr exp, env_ptr env);

#endif /* REPL_HPP */

