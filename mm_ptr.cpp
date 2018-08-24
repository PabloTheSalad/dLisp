#include "mm_ptr.hpp"
#include "types/lisp_t.hpp"

template<>
lisp_t* obj_ptr::object() {
    if (null) return nullptr;
    return get_memory_manager()->get_object(own_obj);
}

obj_ptr get_object(lisp_t* obj) {
    return obj_ptr(get_memory_manager()->get_index(obj));
}

env_ptr make_env(const environment_t&& env) {
    return env_ptr(get_memory_manager()->allocate_object(lisp_t(T_ENV, env)));
}

template<> 
environment_t* env_ptr::object() {
    if (null) return nullptr;
    return get_memory_manager()->get_object(own_obj)->env;
}
