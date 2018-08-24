#include "lisp_types.hpp"
#include "exceptions.hpp"
#include "types/lisp_t.hpp"
#include "mm_ptr.hpp"

template <>
lisp_t::lisp_t <environment_t> (lisp_type_flag type, const environment_t& value)
    : type(type) {
    env = new environment_t(value);
}

template <>
lisp_t::lisp_t <string_t> (lisp_type_flag type, const string_t& value)
    : type(type) {
    string_ptr = new string_t(value);
}

template <>
lisp_t::lisp_t <procedure_t> (lisp_type_flag type, const procedure_t& value)
    : type(type) {
    proc = new procedure_t(value);
}

template <>
lisp_t::lisp_t <pair_t> (lisp_type_flag type, const pair_t& value)
    : type(type) {
    pair = new pair_t(value);
}

//очень опасное место, надо переделать
lisp_t& lisp_t::operator = (const lisp_t& other) {
    type = other.type;
    ref_counter = other.ref_counter;
    switch (other.type) {
        case T_STRING:
        case T_SYMBOL:
            string_ptr = other.string_ptr;
            break;
        case T_PROC:
            proc = other.proc;
            break;
        case T_ENV:
            env = other.env;
            break;
        default:
            memcpy(this, &other, sizeof(lisp_t));
    }
    return *this;
}

lisp_t::lisp_t (const lisp_t& other) {
    *this = other;
}

bool lisp_t::operator == (const lisp_t& other) {
    if (type != other.type) return false;
    switch (type) {
        case T_NULL:
            return true;
        case T_BOOL:
            return boolean == other.boolean;
        case T_NUMBER:
            return number == other.number;
        case T_SPECIAL:
            return spec == other.spec;
        case T_STRING:
        case T_SYMBOL:
            return *string_ptr == *other.string_ptr;
        case T_ENV:
            return env == other.env;
        case T_PROC:
            return proc == other.proc;
        case T_PAIR:
            return pair == other.pair;
    }
}

bool lisp_t::operator < (const lisp_t& other) {
    if (type != other.type) return type < other.type;
    switch (type) {
        case T_BOOL:
            return true;
        case T_NUMBER:
            return number < other.number;
        case T_SPECIAL:
            return true;
        case T_STRING:
            return string_ptr < other.string_ptr;
        case T_SYMBOL:
            return symbol_ptr < other.symbol_ptr;
        case T_ENV:
            return true;
        case T_PROC:
        case T_PAIR:
            return true;
        case T_NULL:
            return true;
    }
}

void lisp_t::append(obj_ptr obj) {
    assert(type == T_PAIR or type == T_NULL, "bad type for append");
    
    if (pair->cdr->type == T_NULL) {
        pair->cdr = singleton_list(obj);
    } else pair->cdr->append(obj);
}

obj_ptr singleton_list(obj_ptr obj) {
    if (obj.is_null()) return empty_list();
    auto list = make_object(T_PAIR, pair_t(obj, empty_list()));
    return list;
}

obj_ptr empty_list() {
    return make_object(T_NULL, 0);
}

bool check_list_type(lisp_type_flag type, obj_ptr list) {
    if(list->type != T_PAIR and list->type != T_NULL) return false;
    for (; list->type != T_NULL; list = list->pair->cdr) {
        if(list->pair->car->type != type) return false;
    }
    return true;
}

obj_ptr lisp_t::end() {
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

size_t lisp_t::len() {
    obj_ptr ptr = get_object(this);
    int i = 1;
    for (; ptr->type == T_PAIR; ptr = ptr->pair->cdr) i++;
    return i;
}

obj_ptr lisp_t::at(int n) {
    assert(type == T_PAIR, "bad type for at", get_object(this));
    
    if (n < 0) n = len() + n - 1;
    if (n == 0) return pair->car;
    else return pair->cdr->at(n-1);
}

bool lisp_t::is_atom() {
    return type != T_PAIR;
}

bool lisp_t::is_self_evaluating() {
    return type != T_PAIR and type != T_SYMBOL;
}

bool lisp_t::is_true() {
    return !(type == T_BOOL and !boolean);
}

bool lisp_t::is_null() {
    return type == T_NULL;
}

bool lisp_t::is_list() {
    return end()->type == T_NULL;
}

bool lisp_t::is_pair() {
    return type == T_PAIR and end()->type != T_NULL;
}
