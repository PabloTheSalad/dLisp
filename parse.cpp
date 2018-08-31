#include "parse.hpp"
#include "tokenizer.hpp"
#include "exceptions.hpp"
#include "lispTypes.hpp"
#include "MemoryManager.hpp"
#include <cctype>
#include <cstdlib>

obj_ptr parse(TokenList tokens) {
    auto root = singletonList(parseForm(tokens));
    while (!tokens.empty()) {
        root->append(parseForm(tokens));
    }
    return root;
}

obj_ptr parseForm(TokenList& tokens) {
    if (tokens.front().type != T_LEFT_BRACE) {
        if (tokens.front().type == T_RIGHT_BRACE) return obj_ptr();
        auto tmp = tokens.front();
        tokens.erase(tokens.begin());
        if (tmp.value != "'") return parseToken(tmp);
        else {
            auto quote = makeObject(T_SYMBOL, Symbol("quote"));
            auto list = singletonList(quote);
            list->append(parseForm(tokens));
            return list;
        }
    } else {
        tokens.erase(tokens.begin());
        auto list = singletonList(parseForm(tokens));
        while(tokens.front().type != T_RIGHT_BRACE) list->append(parseForm(tokens));
        tokens.erase(tokens.begin());
        return list;
    }
}

obj_ptr parseToken(Token tok) {
    if (tok.type == T_STR) return parseString(tok.value);
    if (tok.value.front() == '#') return parseBool(tok.value);
    if (isdigit(tok.value.front()) or 
            (tok.value.front() == '-' and tok.value.size() > 1 and isdigit(tok.value.at(1)))) 
        return parseNumber(tok.value);
    else return parseSymbol(tok.value);
}

//todo: доделать обработку экранирующих последовательностей (tokenizer?)
obj_ptr parseString(std::string& str) {
    auto obj = makeObject(T_STRING, String(str));
    return obj;
}

obj_ptr parseBool(std::string& str) {
    bool value;
    if (str == "#f") value = 0;
    else if (str == "#t") value = 1;
    else throw parseError("bad symbol: #");
    
    auto obj = makeObject(T_BOOL, Bool(value));
    return obj;
}

obj_ptr parseNumber(std::string& str) {
    bool real = false;
    long double sign = 1;
    if (str.front() == '-') {
        sign = -1;
        str.erase(str.begin());
    }
    for(int ch : str) {
        if (isdigit(ch) == 0 and ch != '.') throw parseError("bad number form");
        if (ch == '.') {
            if (!real) real = true;
            else throw parseError("bad number form");
        }
    }
    long double number;
    NumberTypeFlag type;
    if (real) {
        number = std::stold(str);
        type = T_REAL;
    } else {
        number = std::stoll(str);
        type = T_INT;
    }
    auto obj = makeObject(T_NUMBER, Number(type, sign*number));
    return obj;
}

obj_ptr parseSymbol(std::string& str) {
    auto obj = makeObject(T_SYMBOL, Symbol(str));
    return obj;
}
