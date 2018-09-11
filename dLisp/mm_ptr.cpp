#include "mm_ptr.hpp"
#include "types/LispCell.hpp"

//! Возращает указатель на хранящийся объект
template<>
LispCell* obj_ptr::object() const {
    if (null) return nullptr;
    return &getMemoryManager()->getObject(ownObject);
}

//! Возвращает укмный указатель на объект obj, но только если данный объект
//! принадлежит менеджеру памяти
obj_ptr getObject(LispCell* obj) {
    return obj_ptr(getMemoryManager()->getIndex(obj));
}

//! Создает новое окружение и возращает указатель на него
env_ptr makeEnv(Environment&& env) {
    return env_ptr(getMemoryManager()->allocateCell(LispCell(T_ENV, std::move(env))));
}

//! Возращает указатель на окружение
template<> 
Environment* env_ptr::object() const {
    if (null) return nullptr;
    return &getMemoryManager()->getObject(ownObject).environment();
}
