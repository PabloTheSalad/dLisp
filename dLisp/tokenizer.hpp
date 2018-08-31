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

enum TokenFlag {
    T_LEFT_BRACE,
    T_RIGHT_BRACE,
    T_WORD,
    T_STR
};

struct Token {
    TokenFlag type;
    std::string value;
    Token(TokenFlag type, const char* str) : type(type), value(str) {}
    ~Token() {}
};

using TokenList = std::list<Token>;

TokenList tokenizer(const char*);
std::string tokenListToString (TokenList tokens);

#endif /* TOKENIZER_HPP */

