#include "tokenizer.hpp"
#include <cctype>
#include <iostream>
#include "exceptions.hpp"

/*!
 * \brief Производит лексический анализ и
 * разбивает переданную строку на отдельные токены
 * \param string Строка для разбора
 * \return Список токенов
 */
//TokenList tokenizer(const char* string) {
//    enum {
//        START,
//        WORD,
//        STRING,
//        COMMENT
//    } state = START;

//    std::string buf;
//    TokenList tokens;

//    for (int n = 0; string[n]; n++) {
//        char cur_char = string[n];
//        switch (state) {
//            case START:
//                if (isspace(cur_char)) break;
//                if (cur_char == '(') tokens.push_back(Token(TokenType::LeftBrace, "("));
//                else if (cur_char == ')') tokens.push_back(Token(TokenType::RightBrace, ")"));
//                else if (cur_char == ';') state = COMMENT;
//                else if (cur_char == '\'') tokens.push_back(Token(TokenType::Word, "'"));
//                else if (cur_char == '"') {
//                    state = STRING;
//                    buf.clear();
//                }
//                else {
//                    state = WORD;
//                    buf.clear();
//                    buf.push_back(cur_char);
//                }
//                break;
//            case STRING:
//                if (cur_char == '"') {
//                    state = START;
//                    tokens.push_back(Token(TokenType::String, buf.c_str()));
//                } else buf.push_back(cur_char);
//                break;
//            case COMMENT:
//                if (cur_char == '\n') state = START;
//                break;
//            case WORD:
//                if (isspace(cur_char)
//                    or cur_char == '"' or cur_char == '('
//                        or cur_char == ')' or cur_char == ';') {
//                    state = START;
//                    tokens.push_back(Token(TokenType::Word, buf.c_str()));
//                    n--;
//                } else buf.push_back(cur_char);
//        }
//    }
//    if (state == WORD) {
//        tokens.push_back(Token(TokenType::Word, buf.c_str()));
//    }
//    return tokens;
//}

/*!
 * \brief Читает данные из потока и производит лексический анализ одной формы
 * языка lisp
 * \param in Поток для разбора
 * \return Список токенов
 */
TokenList streamTokenizer(std::istream& in) {
    enum {
        START,
        WORD,
        STRING,
        COMMENT
    } state = START;

    std::string buf;
    TokenList tokens;
    bool inForm = true;
    int deep = 0;

    if (in.peek() == '\n') {
        in.get();
        return tokens;
    }

    while((inForm or deep != 0) and !in.eof()) {
        if (deep < 0) throw lexicalError(")");
        int cur_char = in.get();
        switch (state) {
            case START:
                if (isspace(cur_char)) break;
                if (cur_char == '(') {
                    tokens.push_back(Token(TokenType::LeftBrace, "("));
                    deep++;
                }
                else if (cur_char == ')') {
                    tokens.push_back(Token(TokenType::RightBrace, ")"));
                    deep--;
                }
                else if (cur_char == ';') state = COMMENT;
                else if (cur_char == '\'') tokens.push_back(Token(TokenType::Word, "'"));
                else if (cur_char == '"') {
                    state = STRING;
                    buf.clear();
                }
                else {
                    state = WORD;
                    buf.clear();
                    buf.push_back(cur_char);
                }
                break;
            case STRING:
                if (cur_char == '"') {
                    state = START;
                    inForm = false;
                    tokens.push_back(Token(TokenType::String, buf.c_str()));
                } else buf.push_back(cur_char);
                break;
            case COMMENT:
                if (cur_char == '\n') state = START;
                break;
            case WORD:
                if (isspace(cur_char)
                    or cur_char == '"' or cur_char == '('
                        or cur_char == ')' or cur_char == ';') {
                    state = START;
                    inForm = false;
                    tokens.push_back(Token(TokenType::Word, buf.c_str()));
                    in.unget();
                } else buf.push_back(cur_char);
        }
    }
    if (state == WORD) {
        tokens.push_back(Token(TokenType::Word, buf.c_str()));
    }
    if (deep > 0) {
        throw lexicalError("(");
    } else if (deep < 0) {
        throw lexicalError(")z");
    }
    return tokens;
}

//! Преборазует токен в его строковое представление, используется для отладки
std::string tokenListToString (TokenList tokens) {
    std::string str = "";
    for (Token tok : tokens) {
        if (tok.type == TokenType::String) str += "\"" + tok.value + "\"" + " ";
        else str += tok.value + " ";
    }
    return str;
}
