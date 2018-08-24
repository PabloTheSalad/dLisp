#include "tokenizer.hpp"
#include <cctype>

token_list tokenizer(const char* string) {
    enum {
        START,
        WORD,
        STRING,
        COMMENT
    } state = START;
    
    std::string buf;
    token_list tokens;
    
    for (int n = 0; string[n]; n++) {
        char cur_char = string[n];
        switch (state) {
            case START:
                if (isspace(cur_char)) break;
                if (cur_char == '(') tokens.push_back(token(T_LEFT_BRACE, "("));
                else if (cur_char == ')') tokens.push_back(token(T_RIGHT_BRACE, ")"));
                else if (cur_char == ';') state = COMMENT;
                else if (cur_char == '\'') tokens.push_back(token(T_WORD, "'"));
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
                    tokens.push_back(token(T_STR, buf.c_str()));
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
                    tokens.push_back(token(T_WORD, buf.c_str()));
                    n--;
                } else buf.push_back(cur_char);
        }       
    }
    if (state == WORD) {
        tokens.push_back(token(T_WORD, buf.c_str()));
    }
    return tokens;
}

std::string token_list2string (token_list tokens) {
    std::string str = "";
    for (token tok : tokens) {
        if (tok.type == T_STR) str += "\"" + tok.value + "\"" + " ";
        else str += tok.value + " ";
    }
    return str;
}