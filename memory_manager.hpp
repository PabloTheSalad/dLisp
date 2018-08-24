/*! 
 * \file
 * \author Павел Коваленко
 * \date 23 июля 2018 г., 23:35
 */

#ifndef MEMORY_MANAGER_HPP
#define MEMORY_MANAGER_HPP

#include <cstring>
#include <set>
#include <map>
#include <list>
#include <stack>
#include "object_index.hpp"

//! Размер блока памяти в байтах
const unsigned int BLOCK_SIZE = 512;

//! Кол-во зарезервированных ячеек
const unsigned int RESERVED_IDX = 5;

//! Тип предназначеный для индексации памяти
using index_t = size_t;

struct lisp_t;
class environment_t;

/*!
 * \brief Класс предназначеный для управления памятью
 * 
 * Данный класс предназнначен для управления памятью внутри интерпретатора
 */
class memory_manager {
    size_t alloc_blocks_count = 0;
    std::vector<lisp_t*> mem_blocks;
    index_t _next_index; ///< Следующий доступный индекс памяти
    std::stack<index_t> index_stack; ///< Стек содержащий освобожденные индексы памяти
    object_index* obj_idx; ///< Объектный индекс
    std::stack<index_t> free_cells;

    friend void inc_obj_counter(index_t obj);
    friend void dec_obj_counter(index_t obj);
public:
    memory_manager ();
    index_t allocate_object(const lisp_t&);
    lisp_t* allocate_memory_block(size_t);
    lisp_t* alloc_next_memory_block();
    void expand_memory();
    void delete_object(index_t idx);
    lisp_t* get_object(index_t);
    index_t get_index(lisp_t* obj);
    index_t next_index();
    size_t allocated_blocks_count();
    size_t cells_count();
    size_t free_cells_count();
};

void set_memory_manager(memory_manager* mm);
memory_manager* get_memory_manager();
memory_manager* init_memory_manager();
void inc_obj_counter(index_t obj);
void dec_obj_counter(index_t obj);


#endif /* MEMORY_MANAGER_HPP */

