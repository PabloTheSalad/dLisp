/* 
 * File:   parse.hpp
 * Author: pablo
 *
 * Created on 26 июля 2018 г., 0:53
 */

#ifndef PARSE_HPP
#define PARSE_HPP

#include "tokenizer.hpp"
#include "mm_ptr.hpp"

obj_ptr parse(token_list tokens);
obj_ptr parse_form(token_list& tokens);
obj_ptr parse_token(token tok);
obj_ptr parse_string(std::string& str);
obj_ptr parse_bool(std::string& str);
obj_ptr parse_number(std::string& str);
obj_ptr parse_symbol(std::string& str);

#endif /* PARSE_HPP */

