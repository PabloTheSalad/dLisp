/*! 
 * \file
 * \author Павел Коваленко
 * \date 20 августа 2018 г., 10:47
 */

#ifndef OBJECT_INDEX_HPP
#define OBJECT_INDEX_HPP

#include <vector>

struct lisp_t;
using index_t = size_t;

/*!
 * \brief Класс отвечающий за опрделение существования объекта в менеджере памяти
 * 
 * Данный класс предназначен для определения того хранится ли такой же объект в менеджере памяти
 * или нет, если данный объект уже существует то вместо создания его копии используется
 * ссылка на него
 */
class object_index {
    std::vector<index_t> objects;
    lisp_t* memory;
    index_t null_idx;
    index_t bool_idx[2];
    index_t special_idx[3];
public:
    object_index();
    object_index(lisp_t* m, index_t*);
    index_t find_object(bool&, const lisp_t&);
    void add_index(index_t);
    void delete_index(index_t);
};

#endif /* OBJECT_INDEX_HPP */

