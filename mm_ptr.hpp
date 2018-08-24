/* 
 * File:   mm_ptr.hpp
 * Author: pablo
 *
 * Created on 2 августа 2018 г., 2:24
 */

#ifndef MM_PTR_HPP
#define MM_PTR_HPP

#include <cstddef>

#include "lisp_type_flag.hpp"
#include "memory_manager.hpp"
#include "types/lisp_t.hpp"

using index_t = size_t;

template <class T>
class mm_ptr {
    bool null = true;
    index_t own_obj;
    T* object();
public:
    mm_ptr() : own_obj(0) {}
    mm_ptr(index_t obj);
    mm_ptr(const mm_ptr& m);
    ~mm_ptr();
    mm_ptr& operator = (const mm_ptr& m);
    
    inline bool operator == (const mm_ptr<T>& other) const { 
        return own_obj == other.own_obj;
    }
    
    inline bool operator != (const mm_ptr<T>& other) { 
        return own_obj != other.own_obj;
    }
    
    inline bool operator < (const mm_ptr<T>& other) const {
        return own_obj < other.own_obj;
    }
    
    template<class T2>
    T2 as_type() { return T2(own_obj); }
    
    T* operator->() { return object(); }
    T& operator* () { return *object(); }
    
    bool is_null() const { return null; }
};

using obj_ptr = mm_ptr<lisp_t>;
using env_ptr = mm_ptr<environment_t>;

template <class T>
obj_ptr make_object(lisp_type_flag type, const T&& obj) {
    return obj_ptr(get_memory_manager()->allocate_object(lisp_t(type, obj)));
}

template<class T>
mm_ptr<T>::mm_ptr(const mm_ptr& m) { 
    *this = m;
}

template <class T>
mm_ptr<T>& mm_ptr<T>::operator = (const mm_ptr& m) {
    if (!m.null and m.own_obj > RESERVED_IDX)
        inc_obj_counter(m.own_obj);
    if (!null and own_obj > RESERVED_IDX) 
        dec_obj_counter(own_obj);
    own_obj = m.own_obj;
    null = m.null;
    return *this;
}

template <class T>
mm_ptr<T>::mm_ptr(index_t obj) : own_obj(obj), null(false) {
    inc_obj_counter(own_obj);
}

template <class T>
mm_ptr<T>::~mm_ptr() {
    if (!null and own_obj > RESERVED_IDX)
        get_memory_manager()->delete_object(own_obj);
}
/*
template <class T>
bool operator == (const mm_ptr<T>& lhs, const mm_ptr<T>& rhs) { 
        return lhs == rhs;
}
*/
template<> lisp_t* obj_ptr::object();
obj_ptr get_object(lisp_t* obj);


env_ptr make_env(const environment_t&& env);
template<> environment_t* env_ptr::object();
env_ptr get_env(environment_t* obj);

#endif /* MM_PTR_HPP */

