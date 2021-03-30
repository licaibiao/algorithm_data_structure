//
// park_list.c
//

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "park_list.h"

#define LIST_LOCK    pthread_mutex_lock
#define LIST_UNLOCK  pthread_mutex_unlock

/*
 * Allocate a new list_iterator_t with the given start
 * node. NULL on failure.
 */
list_iterator_t *list_iterator_new_from_node(list_node_t *node, list_direction_t direction) 
{
    list_iterator_t *self;
    if (!(self = LIST_MALLOC(sizeof(list_iterator_t))))
        return NULL;
    self->next = node;
    self->direction = direction;
    return self;
}

/*
 * Return the next list_node_t or NULL when no more
 * nodes remain in the list.
 */
list_node_t *list_iterator_next(list_iterator_t *self) 
{
    list_node_t *curr = self->next;
    if (curr) 
    {
        self->next = self->direction == LIST_HEAD
            ? curr->next
            : curr->prev;
    }
    return curr;
}

/*
 * Free the list iterator.
 */
void list_iterator_destroy(list_iterator_t *self) 
{
    LIST_FREE(self);
    self = NULL;
}

/*
 * Allocate a new list_iterator_t. NULL on failure.
 * Accepts a direction, which may be LIST_HEAD or LIST_TAIL.
 */
list_iterator_t *list_iterator_new(list_t *list, list_direction_t direction) 
{
    list_node_t *node = direction == LIST_HEAD
        ? list->head
        : list->tail;
    return list_iterator_new_from_node(node, direction);
}

/*
 * Allocates a new list_node_t. NULL on failure.
 */
list_node_t *list_node_new(void *val) 
{
    list_node_t *self;
    if (!(self = LIST_MALLOC(sizeof(list_node_t))))
        return NULL;
    self->prev = NULL;
    self->next = NULL;
    self->val = val;
    return self;
}

/*
 * user self define the given node to the list
 * and return the node, NULL on failure.
 */
list_node_t *list_defpush(list_t *self, list_node_t *node) 
{
    if (!node) return NULL;

    list_iterator_t *it = list_iterator_new(self, LIST_HEAD);
    list_node_t *p;

    LIST_LOCK(&self->mutex);
    if (0 == self->len) 
    {
        self->head = self->tail = node;
        node->prev = node->next = NULL;
        ++self->len;
        LIST_UNLOCK(&self->mutex);
        return node;
    }

    if (self->pushmatch) 
    {
        while ((p = list_iterator_next(it))) 
        {
            if (self->pushmatch(self->idx, node->val, p->val)) 
            {
                break;
            }
        }

        if (NULL == p)
        {
            node->prev = self->tail;
            node->next = NULL;
            self->tail->next = node;
            self->tail = node;
            ++self->len;
        }
        else
        {
            if (p == self->head)
            {
                node->next = self->head;
                node->prev = NULL;
                self->head->prev = node;
                self->head = node;
            }
            else if (p == self->tail)
            {
                node->next = self->tail;
                node->prev = self->tail->prev;
                self->tail->prev->next = node;
                self->tail->prev = node;
            }
            else
            {
                node->next = p; 
                node->prev = p->prev;
                p->prev->next = node;
                p->prev = node;
            }
            ++self->len;
        }
    }
    else 
    {
        node->prev = self->tail;
        node->next = NULL;
        self->tail->next = node;
        self->tail = node;
        ++self->len;
    }

    list_iterator_destroy(it);
    LIST_UNLOCK(&self->mutex);
    return node;
}

/*
 * Append the given node to the list
 * and return the node, NULL on failure.
 */
list_node_t *list_rpush(list_t *self, list_node_t *node) 
{
    if (!node) return NULL;

    LIST_LOCK(&self->mutex);
    if (self->len) 
    {
        node->prev = self->tail;
        node->next = NULL;
        self->tail->next = node;
        self->tail = node;
    } 
    else 
    {
        self->head = self->tail = node;
        node->prev = node->next = NULL;
    }

    ++self->len;
    LIST_UNLOCK(&self->mutex);
    return node;
}

/*
 * Prepend the given node to the list
 * and return the node, NULL on failure.
 */
list_node_t *list_lpush(list_t *self, list_node_t *node) 
{
    if (!node) return NULL;

    LIST_LOCK(&self->mutex);
    if (self->len) 
    {
        node->next = self->head;
        node->prev = NULL;
        self->head->prev = node;
        self->head = node;
    } 
    else 
    {
        self->head = self->tail = node;
        node->prev = node->next = NULL;
    }

    ++self->len;
    LIST_UNLOCK(&self->mutex);
    return node;
}

/*
 * Return / detach the last node in the list, or NULL.
 */
list_node_t *list_rpop(list_t *self) 
{
    if (!self->len) return NULL;

    LIST_LOCK(&self->mutex);
    list_node_t *node = self->tail;

    if (--self->len) 
    {
        (self->tail = node->prev)->next = NULL;
    } 
    else 
    {
        self->tail = self->head = NULL;
    }

    node->next = node->prev = NULL;
    LIST_UNLOCK(&self->mutex);
    return node;
}

/*
 * Return / detach the first node in the list, or NULL.
 */
list_node_t *list_lpop(list_t *self) 
{
    if (!self->len) return NULL;

    LIST_LOCK(&self->mutex);
    list_node_t *node = self->head;

    if (--self->len) 
    {
        (self->head = node->next)->prev = NULL;
    } 
    else 
    {
        self->head = self->tail = NULL;
    }

    node->next = node->prev = NULL;
    LIST_UNLOCK(&self->mutex);
    return node;
}

/*
 * Return the node associated to val or NULL.
 */
list_node_t *list_find(list_t *self, void *val) 
{
    list_iterator_t *it = list_iterator_new(self, LIST_HEAD);
    list_node_t *node;

    LIST_LOCK(&self->mutex);
    while ((node = list_iterator_next(it))) 
    {
        if (self->match) 
        {
            if (self->match(self->idx, val, node->val)) 
            {
                list_iterator_destroy(it);
                LIST_UNLOCK(&self->mutex);
                return node;
            }
        } 
        else 
        {
            if (val == node->val) 
            {
                list_iterator_destroy(it);
                LIST_UNLOCK(&self->mutex);
                return node;
            }
        }
    }

    list_iterator_destroy(it);
    LIST_UNLOCK(&self->mutex);
    return NULL;
}

/*
 * Return the node at the given index or NULL.
 */
list_node_t *list_at(list_t *self, int index) 
{
    list_direction_t direction = LIST_HEAD;

    LIST_LOCK(&self->mutex);
    if (index < 0) 
    {
        direction = LIST_TAIL;
        index = ~index;
    }

    if ((unsigned int)index < self->len) 
    {
        list_iterator_t *it = list_iterator_new(self, direction);
        list_node_t *node = list_iterator_next(it);
        while (index--) node = list_iterator_next(it);
        list_iterator_destroy(it);
        LIST_UNLOCK(&self->mutex);
        return node;
    }
    LIST_UNLOCK(&self->mutex);

    return NULL;
}

/*
 * Return the node associated to val or NULL.
 */
void list_iterator(list_t *self) 
{
    list_iterator_t *it = list_iterator_new(self, LIST_HEAD);
    list_node_t *node;

    while ((node = list_iterator_next(it))) 
    {
        if (self->iter) 
            self->iter(self->idx, node->val);
    }
    list_iterator_destroy(it);
    return;
}

/*
 * delete all node in list.
 */
void list_clear(list_t *self) 
{
    if (!self->len) return;

    LIST_LOCK(&self->mutex);
    if (NULL != self)
    {
        list_node_t *next;
        list_node_t *curr = self->head;
        unsigned int len = self->len;

        while (len--) 
        {
            next = curr->next;
            if (self->free) self->free(self->idx, curr->val);
            LIST_FREE(curr);
            curr = next;
        }

        self->len = 0;
    }
    LIST_UNLOCK(&self->mutex);
}

/*
 * Remove the given node from the list, freeing it and it's value.
 */
void list_remove(list_t *self, list_node_t *node) 
{
    if (NULL == node) return;

    LIST_LOCK(&self->mutex);
    node->prev
        ? (node->prev->next = node->next)
        : (self->head = node->next);

    node->next
        ? (node->next->prev = node->prev)
        : (self->tail = node->prev);

    if (self->free) self->free(self->idx, node->val);

    --self->len;
    LIST_UNLOCK(&self->mutex);
}

/*
 * Free the list.
 */
void list_destroy(list_t *self) 
{
    if (NULL != self)
    {
        LIST_LOCK(&self->mutex);
        list_node_t *next;
        list_node_t *curr = self->head;
        unsigned int len = self->len;

        while (len--) 
        {
            next = curr->next;
            if (self->free) self->free(self->idx, curr->val);
            LIST_FREE(curr);
            curr = next;
        }
        LIST_UNLOCK(&self->mutex);
        pthread_mutex_destroy(&self->mutex);
        LIST_FREE(self);
    }
}

/*
 * Allocate a new list_t. NULL on failure.
 */
list_t *list_new(void) 
{
    list_t *self = NULL;

    if (!(self = LIST_MALLOC(sizeof(list_t))))
    {
        return NULL;
    }

    memset(self, 0, sizeof(list_t));
    pthread_mutex_init(&self->mutex, NULL);
    self->len   = 0;
    self->head  = NULL;
    self->tail  = NULL;
    self->free  = NULL;
    self->iter  = NULL;
    self->match = NULL;
    self->pushmatch  = NULL;
    return self;
}

