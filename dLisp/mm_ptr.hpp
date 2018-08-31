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

template <class T>
class mm_ptr {
    bool null = true;
    index_t ownObject;
    T* object() const;
public:
    mm_ptr() : ownObject(0) {}
    mm_ptr(index_t obj);

    mm_ptr(const mm_ptr& m);
    mm_ptr& operator = (const mm_ptr& m);

    mm_ptr(mm_ptr&& m) : ownObject(m.ownObject), null(m.null) { m.null = true; }
    mm_ptr& operator = (mm_ptr&& m);
    ~mm_ptr();
    
    inline bool operator == (const mm_ptr<T>& other) const { 
        return ownObject == other.ownObject;
    }
    
    inline bool operator != (const mm_ptr<T>& other) const {
        return ownObject != other.ownObject;
    }
    
    inline bool operator < (const mm_ptr<T>& other) const {
        return ownObject < other.ownObject;
    }
    
    template<class T2>
    T2 as_type() const { return T2(ownObject); }
    
    T* operator->() const { return object(); }
    T& operator* () const { return *object(); }
    
    bool is_null() const { return null; }
};

using obj_ptr = mm_ptr<LispCell>;
using env_ptr = mm_ptr<Environment>;

template <class T>
obj_ptr makeObject(LispTypeFlag type, T&& obj) {
    return obj_ptr(getMemoryManager()->allocateObject(LispCell(type, obj)));
}

template<class T>
mm_ptr<T>::mm_ptr(const mm_ptr& m) : ownObject(m.ownObject), null(m.null) {
    if (!null) getMemoryManager()->signalCreateObject(ownObject);
}

template <class T>
mm_ptr<T>& mm_ptr<T>::operator = (const mm_ptr& m) {
    if (!m.null) getMemoryManager()->signalCreateObject(m.ownObject);
    if (!null) getMemoryManager()->signalDeleteObject(ownObject);
    ownObject = m.ownObject;
    null = m.null;
    return *this;
}

template <class T>
mm_ptr<T>& mm_ptr<T>::operator = (mm_ptr&& m) {
    std::swap(ownObject, m.ownObject);
    std::swap(null, m.null);
    return *this;
}

template <class T>
mm_ptr<T>::mm_ptr(index_t obj) : null(false), ownObject(obj) {
    if (!null) getMemoryManager()->signalCreateObject(ownObject);
}

template <class T>
mm_ptr<T>::~mm_ptr() {
    if (!null) getMemoryManager()->signalDeleteObject(ownObject);
}

template<> LispCell* obj_ptr::object() const;
obj_ptr getObject(LispCell* obj);


env_ptr makeEnv(const Environment&& env);
template<> Environment* env_ptr::object() const;
env_ptr getEnv(Environment* obj);

#endif /* MM_PTR_HPP */

