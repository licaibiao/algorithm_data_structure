
#ifndef __LIST_H__
#define __LIST_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>

#ifndef LIST_MALLOC
#define LIST_MALLOC malloc
#endif

#ifndef LIST_FREE
#define LIST_FREE free
#endif

#pragma pack(push, 1)
/*
 * list_t iterator direction.
 */

typedef enum 
{
    LIST_HEAD,
    LIST_TAIL
} list_direction_t;

/*
 * list_t node struct.
 */

typedef struct list_node 
{
    struct list_node *prev;
    struct list_node *next;
    void *val;
} list_node_t;

/*
 * list_t struct.
 */
typedef struct _list_t
{
    list_node_t *head;
    list_node_t *tail;

    unsigned int idx;   //索引，用于多链表场景
    unsigned int len;   //结点总数
    pthread_mutex_t mutex;

    //钩子函数
    void (*free)(uint32_t idx, void *val);          //自定义释放资源函数
    void (*iter)(uint32_t idx, void *val);          //迭代遍历所有结点
    int (*match)(uint32_t idx, void *val1, void *val2);   //自定义查找匹配函数 
    int (*pushmatch)(uint32_t idx, void *new_val, void *cur_val);   //自定义插入规则 
} list_t;

/*
 * list_t iterator struct.
 */

typedef struct 
{
    list_node_t *next;
    list_direction_t direction;
} list_iterator_t;
#pragma pack(pop)

list_iterator_t *list_iterator_new_from_node(list_node_t *node, list_direction_t direction);
list_iterator_t *list_iterator_new(list_t *list, list_direction_t direction);
list_node_t *list_iterator_next(list_iterator_t *self);
void list_iterator_destroy(list_iterator_t *self);

list_node_t *list_node_new(void *val);
list_node_t *list_defpush(list_t *self, list_node_t *node);
list_node_t *list_rpush(list_t *self, list_node_t *node);
list_node_t *list_lpush(list_t *self, list_node_t *node); 
list_node_t *list_rpop(list_t *self); 
list_node_t *list_lpop(list_t *self); 
list_node_t *list_find(list_t *self, void *val); 
list_node_t *list_at(list_t *self, int index); 

void list_clear(list_t *self);
void list_iterator(list_t *self); 
void list_remove(list_t *self, list_node_t *node); 
void list_destroy(list_t *self); 
list_t *list_new(void); 

#ifdef __cplusplus
}
#endif

#endif

