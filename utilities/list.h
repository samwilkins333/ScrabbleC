#pragma once

/*
 * Generic circular doubly linked list implementation.
 *
 * list_t is the head of the list.
 * list_link_t should be included in structures which want to be
 * linked on a list_t.
 *
 * All of the list functions take pointers to list_t and list_link_t
 * types, unless otherwise specified.
 *
 * list_init(list) initializes a list_t to an empty list.
 *
 * list_empty(list) returns 1 iff the list is empty.
 *
 * Insertion functions.
 *   list_insert_head(list, link) inserts at the front of the list.
 *   list_insert_tail(list, link) inserts at the end of the list.
 *   list_insert_before(olink, nlink) inserts nlink before olink in list.
 *
 * Removal functions.
 * Head is list->l_next.  Tail is list->l_prev.
 * The following functions should only be called on non-empty lists.
 *   list_remove(link) removes a specific element from the list.
 *   list_remove_head(list) removes the first element.
 *   list_remove_tail(list) removes the last element.
 *
 * Item accessors.
 *   list_item(link, type, member)
 * Given a list_link_t* and the name of the type of structure which contains
 * the list_link_t and the name of the member corresponding to the list_link_t,
 * returns a pointer (of type "type*") to the item.
 *
 * To iterate over a list,
 *    list_link_t *link;
 *    for (link = list->l_next;
 *         link != list; link = link->l_next)
 *       ...
 *
 * Or, use the macros, which will work even if you list_remove() the
 * current link:
 *    type iterator;
 *    list_iterate_begin(list, iterator, type, member) {
 *        ... use iterator ...
 *    } list_iterate_end();
 */

#define LIST_INITIALIZER(list) {.l_next = &(list), .l_prev = &(list)}
#define LIST_LINK_INITIALIZER(list_link) {.l_next = NULL, .l_prev = NULL}

typedef struct list {
    struct list *l_next;
    struct list *l_prev;
} list_t, list_link_t;

void list_link_init(list_link_t *link);

void list_init(list_t *list);

long list_link_is_linked(const list_link_t *link);

long list_empty(const list_t *list);

void list_insert_before(list_link_t *link, list_link_t *to_insert);

void list_insert_head(list_t *list, list_link_t *link);

void list_insert_tail(list_t *list, list_link_t *link);

void list_remove(list_link_t *link);

#define list_item(link, type, member)                                   \
        (type*)((char*)(link) - offsetof(type, member))

#define list_head(list, type, member)                                   \
        list_item((list)->l_next, type, member)

#define list_next(current, type, member) list_head(&(current)->member, type, member)
#define list_prev(current, type, member) list_tail(&(current)->member, type, member)

#define list_tail(list, type, member)                                   \
        list_item((list)->l_prev, type, member)

#define list_iterate(list, var, type, member)                                    \
    for (type *var = list_head(list, type, member),                              \
            *__next_ ## var = list_next(var, type, member);                      \
            &var->member != (list);                                              \
            var = __next_ ## var, __next_ ## var = list_next(var, type, member))


#define list_iterate_reverse(list, var, type, member)                            \
    for (type *var = list_tail(list, type, member),                              \
            *__next_ ## var = list_prev(var, type, member);                      \
            &var->member != (list);                                              \
            var = __next_ ## var, __next_ ## var = list_prev(var, type, member))
