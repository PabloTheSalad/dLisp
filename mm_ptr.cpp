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

env_ptr makeEnv(const Environment&& env) {
    return env_ptr(getMemoryManager()->allocateObject(LispCell(T_ENV, env)));
}

template<> 
Environment* env_ptr::object() const {
    if (null) return nullptr;
    return getMemoryManager()->getObject(ownObject).env;
}
