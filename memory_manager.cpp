#include <cstddef>

#include "memory_manager.hpp"
#include "types/lisp_t.hpp"
#include "repl.hpp"
#include "exceptions.hpp"
#include <cstdlib>
#include <iostream>
#include <functional>
#include <stack>

//! Cсылка на глобальный менеджер памяти
static memory_manager* MM;

//! Возвращает ссылку на глобальный менеджер памяти
memory_manager* get_memory_manager() {
    return MM;
}

//! Устанавливает глобальный менеджер памяти
void set_memory_manager(memory_manager* mm) {
    MM = mm;
}

/*!
 * \brief Конструктор менеджера памяти
 * 
 * В данном конструкторе выделяется память для блока памяти 
 */
memory_manager::memory_manager() :  mem_blocks(64), index_stack() {
    _next_index = RESERVED_IDX + 1;
    lisp_t* new_block = alloc_next_memory_block();
    new_block[0] = lisp_t(T_NULL, 0);
    new_block[1] = lisp_t(T_BOOL, bool_t(0));
    new_block[2] = lisp_t(T_BOOL, bool_t(1));
    new_block[3] = lisp_t(T_SPECIAL, special_t(UNDEF));
    new_block[4] = lisp_t(T_SPECIAL, special_t(INF));
    new_block[5] = lisp_t(T_SPECIAL, special_t(NAN));
    index_t obj_arr[6] = {0,1,2,3,4,5};
    obj_idx = new object_index(new_block, obj_arr);
}

lisp_t* memory_manager::allocate_memory_block(size_t idx) {
    lisp_t* new_block = new lisp_t[BLOCK_SIZE];
    mem_blocks[idx] = new_block;
    return new_block;
}

lisp_t* memory_manager::alloc_next_memory_block() {
    return allocate_memory_block(alloc_blocks_count++);
}

void memory_manager::expand_memory() {
    alloc_next_memory_block();
}

size_t memory_manager::allocated_blocks_count() {
    return alloc_blocks_count;
}

size_t memory_manager::cells_count() {
    return _next_index - index_stack.size();
}

size_t memory_manager::free_cells_count() {
    size_t count = 0;
    for (size_t i = 0; i < _next_index; i++) {
        if (get_object(i)->ref_counter == 0) count++;
    }
    return count;
}

memory_manager* init_memory_manager() {
    MM = new memory_manager();
    return MM;
}

void inc_obj_counter(index_t idx) {
    MM->get_object(idx)->ref_counter++;
}

void dec_obj_counter(index_t idx) {
    MM->get_object(idx)->ref_counter--;
}

index_t memory_manager::next_index() {
    if (free_cells.empty()) {
        index_t buf = _next_index;
        _next_index++;
        if (buf >= alloc_blocks_count * BLOCK_SIZE) expand_memory();
        return buf;
    } else {
        index_t buf = free_cells.top();
        free_cells.pop();
        return buf;
    }
}

index_t memory_manager::allocate_object(const lisp_t& obj) {
    bool p = false;
    index_t idx;
    bool typep = obj.type != T_PAIR and obj.type != T_ENV and obj.type != T_PROC;
    if (typep) {
        idx = obj_idx->find_object(p, obj);
        if (p) {
            get_object(idx)->ref_counter++;
            return idx;
        }
    }
    idx = next_index();
    *get_object(idx) = obj;
    get_object(idx)->ref_counter = 0;
    if (typep) obj_idx->add_index(idx);
    return idx;
}

void memory_manager::delete_object(index_t idx) {
    if (idx > RESERVED_IDX) {
        dec_obj_counter(idx);
    }
}

lisp_t* memory_manager::get_object(index_t idx) {
    if (idx > BLOCK_SIZE * alloc_blocks_count) {
        std::cout << "WTF!";
    }
    return mem_blocks[idx/BLOCK_SIZE] + (idx % BLOCK_SIZE);
}

index_t memory_manager::get_index(lisp_t* obj) {
    for (size_t i = 0; i < alloc_blocks_count; i++) {
        if (obj >= mem_blocks[i] and obj < mem_blocks[i] + BLOCK_SIZE) {
            return (BLOCK_SIZE * i) + static_cast<index_t>(obj - mem_blocks[i]);
        }
    }
    throw memory_error("Bad memory pointer");
}
