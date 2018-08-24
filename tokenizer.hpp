/* 
 * File:   tokenizer.hpp
 * Author: pablo
 *
 * Created on 19 июля 2018 г., 21:23
 */

#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
#include <vector>
#include <list>

enum token_flag {
    T_LEFT_BRACE,
    T_RIGHT_BRACE,
    T_WORD,
    T_STR
};

struct token {
    token_flag type;
    std::string value;
    token(token_flag type, const char* str) : type(type), value(str) {}
    ~token() {}
};

using token_list = std::list<token>;

token_list tokenizer(const char*);
std::string token_list2string (token_list tokens);

#endif /* TOKENIZER_HPP */

