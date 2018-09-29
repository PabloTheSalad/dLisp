/* 
 * File:   mm_ptr.hpp
 * Author: pablo
 *
 * Created on 2 августа 2018 г., 2:24
 */

#ifndef MM_PTR_HPP
#define MM_PTR_HPP

#include <cstddef>

#include "LispTypeFlag.hpp"
#include "MemoryManager.hpp"
#include "types/LispCell.hpp"

using index_t = size_t;

/*!
 * \brief Класс представляющий умный указатель для MemoryManager'а
 *
 * Благодаря данному классу происходит подсчет ссылок на объекты в
 * менеджере памяти и их своевременное удаление
 */
template <class T>
class mm_ptr {
    bool null = true;
    LispCell* ownObject;
    T* object() const;
public:
    mm_ptr() : ownObject(nullptr) {}
//    mm_ptr(index_t obj);
    mm_ptr(LispCell& obj);

    mm_ptr(const mm_ptr& m);
    mm_ptr& operator = (const mm_ptr& m);

    mm_ptr(mm_ptr&& m) : null(m.null), ownObject(m.ownObject) { m.null = true; }
    mm_ptr& operator = (mm_ptr&& m);
    ~mm_ptr();
    
    inline bool operator == (const mm_ptr& other) const {
        return ownObject == other.ownObject;
    }
    
    inline bool operator != (const mm_ptr& other) const {
        return ownObject != other.ownObject;
    }
    
    inline bool operator < (const mm_ptr& other) const {
        return ownObject < other.ownObject;
    }
    
    template<class T2>
    T2 as_type() { return T2(reinterpret_cast<LispCell&>(ownObject)); }
    
    T* operator->() const { return object(); }
    T& operator* () const { return *object(); }
    T* get() const { return object(); }
    
    bool isValid() const { return !null; }
};

using obj_ptr = mm_ptr<LispCell>;
using env_ptr = mm_ptr<Environment>;

//! Создает объект в менеджере памяти и возвращает указатель на него
template <class T>
obj_ptr makeObject(LispTypeFlag type, T&& obj) {
    return obj_ptr(getMemoryManager()->allocateCell(LispCell(type, std::forward<T>(obj))));
}

template<class T>
mm_ptr<T>::mm_ptr(const mm_ptr& m) : ownObject(m.ownObject), null(m.null) {
    if (!null) getMemoryManager()->signalCreateObject(ownObject);
}

template <class T>
mm_ptr<T>& mm_ptr<T>::operator = (const mm_ptr& m) {
    if (!m.null) getMemoryManager()->signalCreateObject(m.ownObject);
    if (!null) getMemoryManager()->signalDeleteObject(ownObject);
    null = m.null;
    ownObject = m.ownObject;
    return *this;
}

template <class T>
mm_ptr<T>& mm_ptr<T>::operator = (mm_ptr&& m) {
    std::swap(null, m.null);
    std::swap(ownObject, m.ownObject);
    return *this;
}

//template <class T>
//mm_ptr<T>::mm_ptr(index_t obj) : null(false), ownObject(obj) {
//    getMemoryManager()->signalCreateObject(ownObject);
//}

template <class T>
mm_ptr<T>::mm_ptr(LispCell& obj) : null(false) {
    ownObject = &obj;
    getMemoryManager()->signalCreateObject(ownObject);
}

template <class T>
mm_ptr<T>::~mm_ptr() {
    if (!null) getMemoryManager()->signalDeleteObject(ownObject);
}

env_ptr makeEnv(Environment&& env);
template<> Environment* env_ptr::object() const;
env_ptr getEnv(Environment* obj);

#endif /* MM_PTR_HPP */

