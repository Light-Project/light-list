/* SPDX-License-Identifier: GPL-2.0-or-later */
#include "list.h"

static inline struct list_head *
list_merge(list_cmp_t cmp, void *data, struct list_head *a, struct list_head *b)
{
    struct list_head *node, **tail = &node;

    for (;;) {
        if (cmp(a, b, data) <= 0) {
            *tail = a;
            tail = &a->next;
            a = a->next;
            if (!a) {
                *tail = b;
                break;
            }
        } else {
            *tail = b;
            tail = &b->next;
            b = b->next;
            if (!b) {
                *tail = a;
                break;
            }
        }
    }

    return node;
}

static inline void
list_finish(list_cmp_t cmp, void *data, struct list_head *head, struct list_head *a, struct list_head *b)
{
    struct list_head *tail = head;
    unsigned int count = 0;

    for (;;) {
        if (cmp(a, b, data) <= 0) {
            tail->next = a;
            a->prev = tail;
            tail = a;
            a = a->next;
            if (!a)
                break;
        } else {
            tail->next = b;
            b->prev = tail;
            tail = b;
            b = b->next;
            if (!b) {
                b = a;
                break;
            }
        }
    }

    tail->next = b;
    do {
        if (unlikely(!++count))
            cmp(b, b, data);
        b->prev = tail;
        tail = b;
        b = b->next;
    } while (b);

    tail->next = head;
    head->prev = tail;
}

void list_qsort(struct list_head *head, list_cmp_t cmp, void *data)
{
    struct list_head *pending = NULL, *node = head->next;
    unsigned int count = 0;

    if (unlikely(node == head->prev))
        return;

    head->prev->next = NULL;

    do {
        struct list_head **tail = &pending;
        size_t bits;

        for (bits = count; bits & 1; bits >>= 1)
            tail = &(*tail)->prev;

        if (likely(bits)) {
            struct list_head *b = *tail, *a = b->prev;
            b = list_merge(cmp, data, a, b);
            b->prev = a->prev;
            *tail = b;
        }

        node->prev = pending;
        pending = node;
        node = node->next;
        pending->next = NULL;
        ++count;
    } while (node);

    node = pending;
    pending = pending->prev;

    for (;;) {
        struct list_head *next = pending->prev;

        if (!next)
            break;
        node = list_merge(cmp, data, pending, node);
        pending = next;
    }

    list_finish(cmp, data, head, pending, node);
}

void list_bsort(struct list_head *head, list_cmp_t cmp, void *data)
{
    struct list_head *walk, *prev, *tmp;
    unsigned int count, number;
    bool swapped, record;

    for (record = swapped = true, number = 1;
         swapped && ({swapped = false; count = 1; true;});
         record = false, --number) {
        prev = walk = head->next;
        list_for_each_continue_safe(walk, tmp, head) {
            if (record)
                number++;
            else if (count++ >= number)
                break;
            if (cmp(prev, walk, data) <= 0)
                prev = walk;
            else {
                list_swap(walk, prev);
                swapped = true;
            }
        }
    }
}
