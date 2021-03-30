#ifndef __PARK_STORAGE_H__
#define __PARK_STORAGE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#define OFFLINE_STORAGE_PATH    "/emmc/offlineinfo.ini"
#define MAX_OFFLINE_INURL_NUM   (5) //离线入库图片最大值
#define MAX_OFFLINE_OUTURL_NUM  (2) //离线出库图片最大值

#pragma pack(push, 1)
typedef struct
{
    uint8_t used;
    char url[256];
    char time[32];      //存储时间
} URL_S;

typedef struct
{
    uint64_t serialid;  //序列号
    uint32_t distance;  //超声波距离

    uint8_t incnturl;   //入库图片个数
    uint8_t outcnturl;  //出库图片个数, 采用逻辑位操作

    URL_S inurl[MAX_OFFLINE_INURL_NUM];     //最多存储5个入库文件路径
    URL_S outurl[MAX_OFFLINE_OUTURL_NUM];   //最多存储2个出库文件，第一个存储last图片，第二个存储out图片
} STORAGE_INFO_t;
#pragma pack(pop)

/* 
* @brief 从头依次获取数据结点(类似结点出栈)，
* @param [IN]
* @param [OUT]
* @return 成功:数据结点指针 失败：NULL
*/
STORAGE_INFO_t *park_storage_get_information_fromhead(void);

/* 
* @brief 根据索引获取数据结点
* @param [IN]index 索引
* @param [OUT]
* @return 成功:数据结点指针 失败：NULL
*/
STORAGE_INFO_t *park_storage_get_information(int index);

/* 
* @brief 根据序列号ID查找数据结点
* @param [IN]serialid 序列号
* @param [OUT]
* @return 成功:数据结点指针 失败：NULL
*/
STORAGE_INFO_t *park_storage_found_information(uint64_t serialid);

/* 
* @brief 将数据存储到存储链表
* @param [IN]info 数据指针
* @param [OUT]
* @return 0:成功 -1:失败
*/
int park_storage_insert_information(STORAGE_INFO_t *info);

/* 
* @brief 根据数据内容删除对应的数据结点
* @param [IN]info 数据指针
* @param [OUT]
* @return 0:成功 -1:失败
*/
int park_storage_del_information(STORAGE_INFO_t *info);

/* 
* @brief 根据索引删除对应的数据结点
* @param [IN] index 索引
* @param [OUT]
* @return 0:成功 -1:失败
*/
int park_storage_del_byindex(int index);

/* 
* @brief 获取存储信息的数据个数
* @param [IN]
* @param [OUT]
* @return 0:成功 -1:失败
*/
int park_storage_get_length(void);

/* 
* @brief 输出调试信息，具体输出内容在storage_iter中定义
* @param [IN]
* @param [OUT]
* @return 0:成功 -1:失败
*/
int park_storage_print_info(void);

/* 
* @brief 将保存在文件中的数据加载到内存中
* @param [IN]
* @param [OUT]
* @return 0:成功 -1:失败
*/
int park_storage_fileload(void);

/* 
* @brief 将内存中的数据写入到文件中，系统重启或者进入低功耗前调用，否则数据将会丢失
* @param [IN]
* @param [OUT]
* @return 0:成功 -1:失败
*/
int park_storage_filesave(void);

/* 
* @brief 存储模块去初始化 
* @param [IN]
* @param [OUT]
* @return 0:成功 -1:失败
*/
int park_storage_deinit(void);

/* 
* @brief 存储模块初始化 
* @param [IN]
* @param [OUT]
* @return 0:成功 -1:失败
*/
int park_storage_init(void);

#ifdef __cplusplus
}
#endif

#endif
