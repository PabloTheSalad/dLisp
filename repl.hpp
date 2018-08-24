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

std::string obj_as_str(obj_ptr, bool in_list = false);
void repl();
void repl_command(const char*);
void load_file(const char* filename, env_ptr env);

#endif /* REPL_HPP */

