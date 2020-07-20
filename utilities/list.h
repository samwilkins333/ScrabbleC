#pragma once

typedef struct list_link {
    struct list_link *l_next;
    struct list_link *l_prev;
} list_link_t;

typedef struct list {
    list_link_t anchor;
    size_t size;
} list_t;

#define offsetof(type, member) \
        ((uintptr_t)((char*)&((type *)(0))->member - (char*)0))

void list_init(list_t *list);

long list_empty(const list_t *list);

void list_insert_before(list_link_t *link, list_link_t *to_insert);

void list_insert_tail(list_t *list, list_link_t *link);

void list_remove(list_t *list, list_link_t *link);

#define list_item(link, type, member)                                   \
        (type*)((char*)(link) - offsetof(type, member))

#define list_head(list, type, member)                                   \
        list_item((list)->l_next, type, member)

#define list_next(current, type, member) list_head(&(current)->member, type, member)

#define list_tail(list, type, member)                                   \
        list_item((list)->l_prev, type, member)

#define list_iterate(list, var, type, member)                                    \
    for (type *var = list_head(list, type, member),                              \
            *__next_ ## var = list_next(var, type, member);                      \
            &var->member != (list);                                              \
            var = __next_ ## var, __next_ ## var = list_next(var, type, member))