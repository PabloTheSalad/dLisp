#include "mm_ptr.hpp"
#include "types/LispCell.hpp"

template<>
LispCell* obj_ptr::object() const {
    if (null) return nullptr;
    return &getMemoryManager()->getObject(ownObject);
}

obj_ptr getObject(LispCell* obj) {
    return obj_ptr(getMemoryManager()->getIndex(obj));
}

env_ptr makeEnv(Environment&& env) {
    return env_ptr(getMemoryManager()->allocateCell(LispCell(T_ENV, std::move(env))));
}

template<> 
Environment* env_ptr::object() const {
    if (null) return nullptr;
    return getMemoryManager()->getObject(ownObject).env;
}
