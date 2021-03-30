#include "park_list.h"
#include "park_storage.h"

#define FILE_HEAD_SEGMENT   "WEP01"
#define MAX_OFFLINE_CNT     (1)

static list_t *g_storage_list = NULL;

static int storage_match(uint32_t idx, void *a, void *b)
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

static void storage_free(uint32_t idx, void *val)
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

static void storage_iter(uint32_t idx, void *val)
{
    if (NULL == val)
    {
        return;
    }

    //用户自定义打印规则
    
    STORAGE_INFO_t *p = (STORAGE_INFO_t *)val;

    printf("========= serialid=%lld, incnturl=%d, outcnturl=%d.\n", p->serialid, p->incnturl, p->outcnturl);
}

static int storage_save_info(list_t *self)
{
    list_node_t *node;
    FILE *fp = NULL;

    if (self->len <= 0)
    {
        remove(OFFLINE_STORAGE_PATH);
        return 0;
    }

    fp = fopen(OFFLINE_STORAGE_PATH, "wb");
    if (NULL == fp)
    {
        printf("fopen %s err.\n", OFFLINE_STORAGE_PATH);
        return -1;
    }

    //写文件头
    char *head = FILE_HEAD_SEGMENT;
    fwrite(head, 1, strlen(head), fp);

    //写内容
    list_iterator_t *it = list_iterator_new(self, LIST_HEAD);
    while ((node = list_iterator_next(it))) 
    {
        printf("======OFFline_Save=====id=%lld.\n", ((STORAGE_INFO_t *)(node->val))->serialid);
        fwrite(node->val, 1, sizeof(STORAGE_INFO_t), fp);
    }

    list_iterator_destroy(it);
    fclose(fp);
    sync();

    return 0;
}

static int storage_load_info(list_t *self)
{
    //等待emmc挂载
    int sleep_cnt = 200; //2s
    while (sleep_cnt--)
    {
        if (access(OFFLINE_STORAGE_PATH, F_OK) == 0)
        {
            break;
        }
        usleep(10*1000);
    }

    if (access(OFFLINE_STORAGE_PATH, F_OK) != 0)
    {
        printf("file not exsit.\n");
        return -1;
    }
    
    int i;
    int len = 0;
    FILE *fp = NULL;

    fp = fopen(OFFLINE_STORAGE_PATH, "rb");
    if (NULL == fp)
    {
        printf("fopen %s err.\n", OFFLINE_STORAGE_PATH);
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    //判定总长度
    if (len < strlen(FILE_HEAD_SEGMENT)+sizeof(STORAGE_INFO_t))
    {
        printf("file len err.\n");
        fclose(fp);
        return -1;
    }

    char head[8] = {0};
    len = fread(head, 1, strlen(FILE_HEAD_SEGMENT), fp);
    if (len != strlen(FILE_HEAD_SEGMENT))
    {
        printf("read file head err.\n");
        fclose(fp);
        return -1;
    }
    head[len] = '\0';

    //比较头部字段
    if (0 != strcmp(head, FILE_HEAD_SEGMENT))
    {
        printf("compare file head err, %s.\n", head);
        fclose(fp);
        return -1;
    }

    //一次读取1000个内容
    int max_len = 1000*sizeof(STORAGE_INFO_t);
    uint8_t *p = (uint8_t *)calloc(1, max_len);
    if (NULL == p)
    {
        printf("malloc err.\n");
        fclose(fp);
        return -1;
    }

    while (1)
    {
        len = fread(p, 1, max_len, fp);
        if (len <= 0)
        {
            break;
        }

        for (i = 0; i < len; i += sizeof(STORAGE_INFO_t))
        {
            park_storage_insert_information((STORAGE_INFO_t *)(p+i));
        }
    }

    fclose(fp);
    free(p);
    p = NULL;

    return 0;
}

static void *storage_handle_thread(void* arg)
{
    g_storage_list = list_new();
    if (NULL == g_storage_list)
    {
        return NULL;
    }

    //钩子函数注册
    g_storage_list->match = storage_match;
    g_storage_list->free  = storage_free;
    g_storage_list->iter  = storage_iter;

    park_storage_fileload();
    park_storage_print_info();

    printf("park_storage_init ok.\n");
    return NULL;
}

STORAGE_INFO_t *park_storage_get_information_fromhead(void)
{
    if (!g_storage_list) return NULL;

    list_node_t *node = list_lpop(g_storage_list); //从链表头获取结点, 类似出栈
    if (node)
    {
        list_rpush(g_storage_list, node); //该节点追加至末尾

        return (STORAGE_INFO_t *)node->val;
    }
    return NULL;
}

STORAGE_INFO_t *park_storage_get_information(int index)
{
    if (!g_storage_list) return NULL;

    list_node_t *node = list_at(g_storage_list, index);
    if (node)
    {
        return (STORAGE_INFO_t *)node->val;
    }
    return NULL;
}

STORAGE_INFO_t *park_storage_found_information(uint64_t serialid)
{
    STORAGE_INFO_t info;
    memset(&info, 0, sizeof(info));
    info.serialid = serialid;

    if (!g_storage_list) return NULL;

    list_node_t *node = list_find(g_storage_list, (void *)&info);
    if (node)
    {
        return (STORAGE_INFO_t *)node->val;
    }
    return NULL;
}

int park_storage_insert_information(STORAGE_INFO_t *info)
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

int park_storage_del_information(STORAGE_INFO_t *info)
{
    if (!g_storage_list) return -1;

    list_node_t *node = list_find(g_storage_list, (void *)info);
    if (node)
    {
        list_remove(g_storage_list, node);
    }
    return 0;
}

int park_storage_del_byindex(int index)
{
    if (!g_storage_list) return -1;

    list_node_t *node = list_at(g_storage_list, index);
    if (node)
    {
        list_remove(g_storage_list, node);
    }
    return 0;
}

int park_storage_get_length(void)
{
    if (!g_storage_list) return -1;

    return g_storage_list->len;
}

int park_storage_print_info(void)
{
    if (!g_storage_list) return -1;

    printf("========= OFFline Total=%d.=========\n", g_storage_list->len);
    list_iterator(g_storage_list);
    printf("========= OFFline End =========\n");
    return 0;
}

int park_storage_fileload(void)
{
    if (!g_storage_list) return -1;

    return storage_load_info(g_storage_list);
}

int park_storage_filesave(void)
{
    if (!g_storage_list) return -1;

    return storage_save_info(g_storage_list);
}

int park_storage_deinit(void)
{
    if (!g_storage_list) return -1;

    list_destroy(g_storage_list);
    return 0;
}

int park_storage_init(void)
{
    if (g_storage_list) return -1; //已初始化

    pthread_t stStoragePid;
    pthread_create(&stStoragePid, NULL, storage_handle_thread, NULL);
    pthread_detach(stStoragePid);  
    return 0;
}
