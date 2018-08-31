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

obj_ptr parse(TokenList tokens);
obj_ptr parseForm(TokenList& tokens);
obj_ptr parseToken(Token tok);
obj_ptr parseString(std::string& str);
obj_ptr parseBool(std::string& str);
obj_ptr parseNumber(std::string& str);
obj_ptr parseSymbol(std::string& str);

#endif /* PARSE_HPP */

