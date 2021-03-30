#include "list.h"
#include "storage.h"

static list_t *g_storage_list = NULL;

#define MAX_OFFLINE_CNT     (65535)

static int storage_hook_match(uint32_t idx, void *a, void *b)
{
    if ((NULL == a) || (NULL == b))
    {
        return 0;
    }
    STORAGE_INFO_t *pa = (STORAGE_INFO_t *)a;
    STORAGE_INFO_t *pb = (STORAGE_INFO_t *)b;

    //用户自定义比较规则
    //这里根据序列号做比较

    if (pa->serialid == pb->serialid)
    {
        return 1;
    }

    return 0;
}


static void storage_hook_free(uint32_t idx, void *val)
{
    int i;
    STORAGE_INFO_t *p = (STORAGE_INFO_t *)val;

    for (i = 0; i < MAX_OFFLINE_INURL_NUM; i++)
    {
        if (access(p->inurl[i].url, F_OK) == 0)
        {
            remove(p->inurl[i].url);
        }
    }
    for (i = 0; i < MAX_OFFLINE_OUTURL_NUM; i++)
    {
        if (access(p->outurl[i].url, F_OK) == 0)
        {
            remove(p->outurl[i].url);
        }
    }

    if (val)
    {
        free(val);
        val = NULL;
    }
}

static void storage_hook_iter(uint32_t idx, void *val)
{
    if (NULL == val)
    {
        return;
    }

    //用户自定义打印规则
    
    STORAGE_INFO_t *p = (STORAGE_INFO_t *)val;

    printf("========= serialid=%lu, incnturl=%d, outcnturl=%d.\n", p->serialid, p->incnturl, p->outcnturl);
}

int storage_insert_information(STORAGE_INFO_t *info)
{
    if (NULL == info) return -1;

    if (!g_storage_list) return -1;

    STORAGE_INFO_t *val = (STORAGE_INFO_t *)calloc(1, sizeof(STORAGE_INFO_t));
    if (val)
    {
        //最大存储100条记录，超过100条之后，从头开始删除结点
        if (g_storage_list->len >= MAX_OFFLINE_CNT)
        {
			return -1; //已经有离线记录，当未清除前，不再存储后面的记录
            //park_storage_del_byindex(0);
        }

        memcpy(val, info, sizeof(STORAGE_INFO_t));
        list_rpush(g_storage_list, list_node_new((void *)val)); //从链表尾插入数据
    }
    return 0;
}

int storage_print_info(void)
{
    if (!g_storage_list) return -1;

    printf("========= OFFline Total=%d.=========\n", g_storage_list->len);
    list_iterator(g_storage_list);
    printf("========= OFFline End =========\n");
    return 0;
}

int storage_deinit(void)
{
    if (!g_storage_list) return -1;

    list_destroy(g_storage_list);
    return 0;
}

int storage_init(void)
{
    if (g_storage_list) return -1; //已初始化
	
	    g_storage_list = list_new();
    if (NULL == g_storage_list)
    {
        return -2;
    }

    //钩子函数注册
    g_storage_list->match = storage_hook_match;
    g_storage_list->free  = storage_hook_free;
    g_storage_list->iter  = storage_hook_iter;

    printf("park_storage_init ok.\n");

    return 0;
}






