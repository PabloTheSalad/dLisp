/*!
 * \file
 * \author Павел Коваленко
 * \date 19 июля 2018 г., 21:23
 */

#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
#include <vector>
#include <list>

//! Типы токенов
enum class TokenType {
    LeftBrace, ///< Левая скобка
    RightBrace, ///< Правая скобка
    Word, ///< Слово
    String ///< Строка
};

//! Структура представляющая токен используемый при разборе кода
struct Token {
    TokenType type;
    std::string value;
    Token(TokenType type, const char* str) : type(type), value(str) {}
    ~Token() {}
};

using TokenList = std::list<Token>;

//TokenList tokenizer(const char*);
TokenList streamTokenizer(std::istream& in);
std::string tokenListToString (TokenList tokens);

#endif /* TOKENIZER_HPP */

