#include "lispTypes.hpp"
#include "exceptions.hpp"
#include "types/LispCell.hpp"
#include "mm_ptr.hpp"

template <>
LispCell::LispCell <Environment> (LispTypeFlag type, const Environment& value)
    : type(type), isMutable(true) {
    env = new Environment(value);
}

template <>
LispCell::LispCell <String> (LispTypeFlag type, const String& value)
    : type(type) {
    string = new String(value);
}

template <>
LispCell::LispCell <Procedure> (LispTypeFlag type, const Procedure& value)
    : type(type) {
    proc = new Procedure(value);
}

template <>
LispCell::LispCell <Pair> (LispTypeFlag type, const Pair& value)
    : type(type), isMutable(true) {
    pair = new Pair(value);
}

bool LispCell::operator == (const LispCell& other) {
    if (type != other.type) return false;
    switch (type) {
        case T_EMPTY:
            return true;
        case T_BOOL:
            return boolean == other.boolean;
        case T_NUMBER:
            return number == other.number;
        case T_SPECIAL:
            return spec == other.spec;
        case T_STRING:
        case T_SYMBOL:
            return *string == *other.string;
        case T_ENV:
            return env == other.env;
        case T_PROC:
            return proc == other.proc;
        case T_PAIR:
            return pair == other.pair;
    }
}

void LispCell::clear() {
    if (type == T_STRING or type == T_SYMBOL) {
        delete string;
    } else if (type == T_ENV) {
        delete env->getSymbols();
        delete env;
    } else if (type == T_PAIR) {
        delete pair;
    } else if (type == T_PROC) {
        delete proc->formalArgs;
        delete proc;
    }
    *this = LispCell();
}

void LispCell::append(obj_ptr obj) {
    assert(type == T_PAIR or type == T_EMPTY, "bad type for append");
    
    if (pair->cdr->type == T_EMPTY) {
        pair->cdr = singletonList(obj);
    } else pair->cdr->append(obj);
}

obj_ptr singletonList(obj_ptr obj) {
    if (obj.is_null()) return emptyList();
    auto list = makeObject(T_PAIR, Pair(obj, emptyList()));
    return list;
}

obj_ptr emptyList() {
    return makeObject(T_EMPTY, 0);
}

bool checkListType(LispTypeFlag type, obj_ptr list) {
    if(list->type != T_PAIR and list->type != T_EMPTY) return false;
    for (; list->type != T_EMPTY; list = list->pair->cdr) {
        if(list->pair->car->type != type) return false;
    }
    return true;
}

obj_ptr LispCell::end() {
    assert(type == T_PAIR, "bad type for end");
    if (pair->cdr->type != T_PAIR) return pair->cdr;
    else return pair->cdr->end();
}

/*
size_t lisp_t::len() {
    if (type != T_PAIR) return 1;
    else return 1 + pair.cdr->len();
}
*/

size_t LispCell::len() {
    obj_ptr ptr = getObject(this);
    int i = 1;
    for (; ptr->type == T_PAIR; ptr = ptr->pair->cdr) i++;
    return i;
}

obj_ptr LispCell::at(int n) {
    assert(type == T_PAIR, "bad type for at", getObject(this));
    
    if (n < 0) n = len() + n - 1;
    if (n == 0) return pair->car;
    else return pair->cdr->at(n-1);
}

bool LispCell::isAtom() {
    return type != T_PAIR;
}

bool LispCell::isSelfEvaluating() {
    return type != T_PAIR and type != T_SYMBOL;
}

bool LispCell::isTrue() {
    return !(type == T_BOOL and !boolean);
}

bool LispCell::isNull() {
    return type == T_EMPTY;
}

bool LispCell::isList() {
    return end()->type == T_EMPTY;
}

bool LispCell::isPair() {
    return type == T_PAIR and end()->type != T_EMPTY;
}
