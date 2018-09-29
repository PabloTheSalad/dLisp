#include "mm_ptr.hpp"
#include "types/LispCell.hpp"
#include "types/Environment.hpp"

//! Возращает указатель на хранящийся объект
template<>
LispCell* obj_ptr::object() const {
    if (null) return nullptr;
    return ownObject;
}

//! Создает новое окружение и возращает указатель на него
env_ptr makeEnv(Environment&& env) {
    return env_ptr(getMemoryManager()->allocateCell(LispCell(T_ENV, std::move(env))));
}

//! Возращает указатель на окружение
template<> 
Environment* env_ptr::object() const {
    if (null) return nullptr;
    return &ownObject->environment();
}
