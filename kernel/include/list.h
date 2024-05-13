#ifndef KERNEL_LIST_H_
#define KERNEL_LIST_H_
#include <stdbool.h>
#include <stddef.h>
#include "types.h"

struct list
{
    struct list* p_next;
    struct list* p_prev;
};

typedef struct list list_t;

#define LIST_CREATE(name)        \
        struct list name = {&name, &name}

#define _list_member_of(ptr, type, member) \
    (type*)((char*)(struct list*)(ptr) - offsetof(type, member))

/**
 * @def wrapper of priavate macro _list_member_of
 * @see _list_member_of
*/
#define list_get_ptr(ptr, type, member) \
    _list_member_of(ptr, type, member)

static inline void list_init(list_t* head)
{
    head->p_next = head;
    head->p_prev = head;
}

static inline void list_deinit(list_t* head)
{
    head->p_next = NULL;
    head->p_prev = NULL;
}

/**
 * @brief private function to add p_new p_entry to list between p_prev and p_next address
*/
static inline void _list_add(list_t* p_new, list_t* p_prev, list_t* p_next) {
    p_prev->p_next = p_new;
    p_next->p_prev = p_new;
    p_new->p_prev = p_prev;
    p_new->p_next = p_next;
}

/**
 * @brief function to check that list is empty
*/
static inline bool list_is_empty(list_t* p_head) {
    return p_head->p_next == p_head;
}

/**
 * @brief function to check p_entry is equal to p_head
*/
static inline bool list_is_head(list_t* p_entry, list_t* p_head) {
    return p_entry == p_head;
}

/**
 * @brief function to check p_entry is first in list
*/
static inline bool list_is_first(list_t* p_entry, list_t* p_head) {
    return p_head->p_next == p_entry;
}

/**
 * @brief function to check p_entry is last in list
*/
static inline bool list_is_last(list_t* p_entry, list_t* p_head) {
    return p_head->p_prev == p_entry;
}

/**
 * @brief function to add element between p_prev and p_next element in list
*/
static inline void list_add_between(list_t* p_new, list_t* p_prev, list_t* p_next) {
    _list_add(p_new, p_prev, p_next);
}

/**
 * @brief function to add element to begin of list
*/
static inline void list_add_head(list_t* p_new, list_t* p_head) {
    _list_add(p_new, p_head, p_head->p_next);
}

/**
 * @brief function to add element to end of list
*/
static inline void list_add_tail(list_t* p_new, list_t* p_head) {
    _list_add(p_new, p_head->p_prev, p_head);
}

/**
 * @brief private function to delete element between p_prev and p_next element in list
*/
static inline void _list_del(list_t* p_prev, list_t* p_next) {
    p_prev->p_next = p_next;
    p_next->p_prev = p_prev;
}

/**
 * @brief function to delete element from list
*/
static inline void list_del_entry(list_t* p_entry) {
    _list_del(p_entry->p_prev, p_entry->p_next);
}

/**
 * @brief function to clear list
*/
static inline void list_del_all(list_t* p_head) {
    p_head->p_next = p_head;
    p_head->p_prev = p_head;
}

/**
 * @brief function to move element from one list to another
*/
static inline void list_move_tail(list_t* p_entry, list_t* new_head) {
    list_del_entry(p_entry);
    list_add_tail(p_entry, new_head);
}

static inline void list_move_head(list_t* p_entry, list_t* new_head) {
    list_del_entry(p_entry);
    list_add_head(p_entry, new_head);
}

/**
 * @brief function to get p_next element from list
*/
static inline list_t* list_next(list_t* p_entry) {
    return p_entry->p_next;
}

/**
 * @brief function to get p_prev element from list
*/
static inline list_t* list_prev(list_t* p_entry) {
    return p_entry->p_prev;
}

/**
 * @brief function to get first element from list
*/
static inline list_t* list_first(list_t* p_head) {
    return p_head->p_next;
}

/**
 * @brief function to get last element from list
*/
static inline list_t* list_last(list_t* p_head) {
    return p_head->p_prev;
}

static inline bool list_is(list_t* entry, list_t* head)
{
    list_t* el = head->p_next;
    while(el != head)
    {
        if(el == entry)
        {
            return true;
        }
        el = list_next(el);
    }
    return false;
}

static inline i32 list_size(list_t* head)
{
    list_t* el = head->p_next;
    i32 size = 0;
    while(el != head)
    {
        size++;
        el = list_next(el);
    }
    return size;
}
#endif /* KERNEL_LIST_H_ */
