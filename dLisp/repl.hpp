/* 
 * File:   repl.hpp
 * Author: pablo
 *
 * Created on 26 июля 2018 г., 1:49
 */

#ifndef REPL_HPP
#define REPL_HPP

#include <iostream>
#include "mm_ptr.hpp"
#include <chrono>

std::string objectAsString(obj_ptr, bool in_list = false);
void repl();
bool loadFile(const char* filename, env_ptr env);
void evalAndPrintString(const char*, env_ptr);
obj_ptr tokenizeAndParse(const char* code);
std::pair<std::chrono::microseconds, obj_ptr> measureEvalExpTime(obj_ptr exp, env_ptr env);

#endif /* REPL_HPP */

