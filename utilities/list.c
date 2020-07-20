#include <stdlib.h>
#include "list.h"

inline void list_init(list_t *list) {
    list->anchor.l_next = list->anchor.l_prev = &list->anchor;
    list->size = 0;
}

inline long list_empty(const list_t *list) {
    return list->size == 0;
}

inline void list_insert_before(list_link_t *link, list_link_t *to_insert) {
    list_link_t *prev = to_insert;
    list_link_t *next = link;
    prev->l_next = next;
    prev->l_prev = next->l_prev;
    next->l_prev->l_next = prev;
    next->l_prev = prev;
}

inline void list_insert_tail(list_t *list, list_link_t *link) {
    list_insert_before(&list->anchor, link);
    list->size++;
}

inline void list_remove(list_t *list, list_link_t *link) {
    list_link_t *ll = link;
    list_link_t *prev = ll->l_prev;
    list_link_t *next = ll->l_next;
    prev->l_next = next;
    next->l_prev = prev;
    ll->l_next = ll->l_prev = NULL;
    list->size--;
}
