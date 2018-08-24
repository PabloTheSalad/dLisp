#include "parse.hpp"
#include "tokenizer.hpp"
#include "exceptions.hpp"
#include "lisp_types.hpp"
#include "memory_manager.hpp"
#include <cctype>
#include <cstdlib>

obj_ptr parse(token_list tokens) {
    auto root = singleton_list(parse_form(tokens));
    while (!tokens.empty()) {
        root->append(parse_form(tokens));
    }
    return root;
}

obj_ptr parse_form(token_list& tokens) {
    if (tokens.front().type != T_LEFT_BRACE) {
        if (tokens.front().type == T_RIGHT_BRACE) return obj_ptr();
        auto tmp = tokens.front();
        tokens.erase(tokens.begin());
        if (tmp.value != "'") return parse_token(tmp);
        else {
            auto quote = make_object(T_SYMBOL, symbol_t("quote"));
            auto list = singleton_list(quote);
            list->append(parse_form(tokens));
            return list;
        }
    } else {
        tokens.erase(tokens.begin());
        auto list = singleton_list(parse_form(tokens));
        while(tokens.front().type != T_RIGHT_BRACE) list->append(parse_form(tokens));
        tokens.erase(tokens.begin());
        return list;
    }
}

obj_ptr parse_token(token tok) {
    if (tok.type == T_STR) return parse_string(tok.value);
    if (tok.value.front() == '#') return parse_bool(tok.value);
    if (isdigit(tok.value.front()) or 
            (tok.value.front() == '-' and tok.value.size() > 1 and isdigit(tok.value.at(1)))) 
        return parse_number(tok.value);
    else return parse_symbol(tok.value);
}

//todo: доделать обработку экранирующих последовательностей (tokenizer?)
obj_ptr parse_string(std::string& str) {
    auto obj = make_object(T_STRING, string_t(str));
    return obj;
}

obj_ptr parse_bool(std::string& str) {
    bool value;
    if (str == "#f") value = 0;
    else if (str == "#t") value = 1;
    else throw parse_error("bad symbol: #");
    
    auto obj = make_object(T_BOOL, bool_t(value));
    return obj;
}

obj_ptr parse_number(std::string& str) {
    bool real = false;
    int sign = 1;
    if (str.front() == '-') {
        sign = -1;
        str.erase(str.begin());
    }
    for(int ch : str) {
        if (isdigit(ch) == 0 and ch != '.') throw parse_error("bad number form");
        if (ch == '.') {
            if (!real) real = true;
            else throw parse_error("bad number form");
        }
    }
    long double number;
    number_type_flag type;
    if (real) {
        number = std::stold(str);
        type = T_REAL;
    } else {
        number = std::stoll(str);
        type = T_INT;
    }
    auto obj = make_object(T_NUMBER, number_t(type, sign*number));
    return obj;
}

obj_ptr parse_symbol(std::string& str) {
    auto obj = make_object(T_SYMBOL, symbol_t(str));
    return obj;
}